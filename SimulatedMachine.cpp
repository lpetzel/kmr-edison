#include "SimulatedMachine.h"
#include <iomanip>
// Check, which we need:
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
//#include <cerrno>
#include <exception>
#include <fstream>
#include <iostream>
#include <ctime>


using namespace std;


template<class T> SimulatedMachine<T>::SimulatedMachine(): report_mode_(REPORT_PUSH),
  in_(nullptr), out_(nullptr) {
    int nr = T::getNrRegisters();
    registers_sps_.reserve(nr);
    for(int i = 0; i < nr; ++i) {
      registers_sps_.push_back(0);
    }
}

template<class T> SimulatedMachine<T>::SimulatedMachine(int nr_registers):
  report_mode_(REPORT_PUSH),
  in_(nullptr), out_(nullptr),
  registers_sps_(nr_registers) {
  for(int i = 0; i < nr_registers; i++) {
    registers_sps_.push_back(0);
  }
}


// do nothing
template<class T> void SimulatedMachine<T>::connectSPS(const std::string&, unsigned short) {
  if (isInitialized())
    getOStream() << "Fake-Initializing the SPS...\n" << std::endl;
}

template<class T> void SimulatedMachine<T>::pushRegisters() {
  std::lock_guard<std::mutex> guard(lock_);
  int nr = T::getNrRegisters();
  bool report = (report_mode_ & REPORT_PUSH) and isInitialized();
  for(int i = 0; i < nr; ++i) {
    if( report and
        (T::registers_.at(i) != registers_sps_.at(i))) {
      getOStream() << "[" << i << "] <- " << T::registers_.at(i) << " (was " <<
        registers_sps_.at(i) << ")" << std::endl;
    }
    registers_sps_[i] = T::registers_.at(i);
  }
}

template<class T> void SimulatedMachine<T>::updateRegisters() {
  std::lock_guard<std::mutex> guard(lock_);
  int nr = T::getNrRegisters();
  bool report = (report_mode_ & REPORT_UPDATE) and isInitialized();
  for(int i = 0; i < nr; ++i) {
    if( report and
        (T::registers_.at(i) != registers_sps_.at(i))) {
      getOStream() << "updated register " << i << std::endl;
    }
    T::registers_[i] = registers_sps_.at(i);
  }
}

template<class T> int SimulatedMachine<T>::getRegister(int i, int) {
  std::lock_guard<std::mutex> guard(lock_);
  int nr = T::getNrRegisters();
  bool report = (report_mode_ & REPORT_UPDATE) and isInitialized();
  if( report and
      (T::registers_.at(i) != registers_sps_.at(i))) {
    getOStream() << "updated register " << i << std::endl;
  }
  T::registers_[i] = registers_sps_.at(i);
  return T::registers_[i];
}

template<class T> void SimulatedMachine<T>::showState() const {
  std::lock_guard<std::mutex> guard(lock_);
  if(not isInitialized())
    return;
  int nr = T::getNrRegisters();
  std::ostream& out = getOStream();
  for(int i = 0; i < nr; ++i) {
    out << right << setw(4) << i
      << left << setw(8) << registers_sps_.at(i);
    if (T::registers_.at(i) != registers_sps_.at(i)) {
      out << "\03333m(" << T::registers_.at(i) <<" in memory)\0330m";
    }
    out << std::endl;
  }
}
  
template<class T> ostream& SimulatedMachine<T>::getOStream() const {
  return *out_;
}

template<class T> istream& SimulatedMachine<T>::getIStream() const {
  return *in_;
}

template<class T> bool SimulatedMachine<T>::isInitialized() const {
  return out_ and in_;
}


/*template<class T> SimulatedMachine<T>::initializeTTY() {
  int fdm;
  int rc;
  fdm = posix_openpt(O_RDWR);
  if (fdm < 0)
    throw exception("Could not open pty");
  rc = grantpt(fdm);
  if(rc != 0)
    throw exception("

}*/

template<class T> void SimulatedMachine<T>::initializeTTY() {
  // I want to open a xterm and get slave access to its pty.
  // Unfortunately, I cannot tell the xterm, which tty to use.
  // Also xterm requires to run a program. It can't be told to
  // start without a command to run.
  //
  // The way to go is:
  // *) Start xterm with a dummy program (and -hold option, so that it won't
  //    close when the dummy program finished)
  // *) The dummy program outputs the slave tty path
  // *) But, because stdout and stdin get redirected to the newly
  //    created tty, the output has to go to a newly created descriptor 3,
  //    which is a pipe. I possess the other side of the pipe, so that
  //    I can read from it
  //

  // Create the pipe:
  int fd[2];
  if( pipe(fd) != 0) {
    //throw exception("Could not create pipe!");
    cerr << "Could not create pipe!" << endl;
    throw exception();
  }

  // 0 -> input -> father keeps it
  // 1 -> output -> child keeps it and makes it nr 3

  // now let's fork
  pid_t pid = fork();
  if (pid == 0) { // Child
    close(fd[0]);
    // Fortunately dup2 does pretty much what we want!
    if(dup2(fd[1], 3) !=3 ) {
      cerr << "Error while duplicating pipe descriptor" <<endl;
    }
    // Now we want the old descriptor to close on exec:
    if(fd[1] != 3) {
      close(fd[1]);
    }
    //fcntl(fd[0], F_SETFD, FD_CLOEXEC);
    // now let's exec!
    // assume executable in current directory
    // TODO: find out, how to get this path efficiently
    const char* args[] = {
      "/bin/env", "xterm", "-hold", "-e", "./simulatedMachineHelper", nullptr};
    execv("/bin/env", (char **) args); // Stupid function does not like chonst
                                       // Why?
    // Still here???
    // Something went terribly wrong...
    // Hopefully daddy will realize, that his son died as a foetus
    write(3, "ERROR", 6);
    close(3);
    exit(1);
  } else if (pid < 0) {
    // An error eh?
    cerr << "Error while forking: pid = " << pid <<endl;
    throw exception();
    //throw exception("Could not fork in order to create xterm!");
  } else {
    // Father process -> lets read slave path from fd[0]
    close(fd[1]);
    {
      char buff[256];
      int num;
      num = read(fd[0], (void *) buff, 255);
      // while(not (num = read(fd[0], (void *) buff, 255))) {
      //   cout << " waiting for child process ..." << endl;
      //   usleep(500000);
      // }
      cout << "Read " << num << " Characters" << endl;
      buff[num] = '\0';
      pty_name_ = buff;
      cout << "TTY on " << pty_name_ << endl;
      close(fd[0]);
    }
    in_ = new ifstream(pty_name_);
    out_ = new ofstream(pty_name_);
    (*out_) << "Hi there on your terminal! Welcome to Simulated Machine\n"
         << "It's super cool, if you can see this message!\n"
         << "This terminal is linked to the pty " << pty_name_
         << " (if you want to do some advanced stuff...)."<<endl;
  }
}

template<class T> void SimulatedMachine<T>::run() {
  // This is better done before calling this method in a separate thread!
  cout << "Running thread..." << endl;
  if(! isInitialized() ) {
    initializeTTY();
  }
  string command;
  while(! out_->eof()) {
    (*out_ ) << "What do you want to do? (h for help)" <<endl;
    (*in_) >> command;
    if (command == "h") {
      (*out_) << "Haha, I played a trick on you.\n"
        << "There is no functionality yet..." <<endl;
    } else {
      (*out_) << "I didn't understand your command "
        << command << ". Please try again." <<endl;
    }
  }
  dynamic_cast<ofstream*> (out_)->close();
  dynamic_cast<ifstream*> (in_)->close();
  delete out_;
  out_ = nullptr;
  delete in_;
  in_ = nullptr;
  pty_name_ = "";
}

template<class T> SimulatedMachine<T>::~SimulatedMachine() {
  delete out_;
  delete in_;
}
  
template class SimulatedMachine<Machine>;
    




  

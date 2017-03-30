#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>



void pipe1() {
  // Create a pipe, read write to the one end and read from the other

  // Create the pipe:
  int fd[2];
  if( pipe(fd) != 0) {
    printf("Could not create pipe...\n");
    exit(1);
  }
  printf("Now writing to pipe...\n");
  dprintf(fd[1], "hi there, what's up\n");
  
  printf("Now close the pipe... \n");
  close(fd[1]);

  printf("Now read the pipe... \n");

  char buff[256];
  int num = read(fd[0], (void *) buff, 255);
  buff[num] = '\0';
  printf("Read %d bytes: %s\n", num, buff);

  printf("Closing the pipe...\n");
  close(fd[0]);

  printf("Now return...\n");
}

void pipe2() {
  // Create a pipe, fork, child writes to pipe, father reads from pipe.

  // Create the pipe:
  int fd[2];
  if( pipe(fd) != 0) {
    printf("Could not create pipe...\n");
    exit(1);
  }

  printf("Now going to fork...\n");
  pid_t pid = fork();
  if(pid == 0) {
    printf("Close read fd...\n");
    close(fd[0]);
    printf("Hello world! What a nice place to live! Let's talk with daddy!\n");
    dprintf(fd[1], "Hi dad, how are you doing?\n");
    printf("Oje, it's time to die. What a short live...\n");
    exit(0);
  } else if (pid > 0) {
    printf("Close write fd...\n");
    close(fd[1]);
    printf("Oh, I became father of %x. That feels great! Let's wait, what he has to say...\n", pid);
    char buff[256];
    int num = read(fd[0], (void *) buff, 255);
    buff[num] = '\0';
    printf("Read %d bytes: %s\n", num, buff);

    printf("Closing the pipe...\n");
    close(fd[0]);
    close(fd[1]);

    printf("Now return...\n");
  } else {
    printf("There was an error while forking! Am I impotent?\n");
  }
}

void pipe3() {
  // Create a pipe, fork, child duplicates file descriptor.
  int fd[2];
  if( pipe(fd) != 0) {
    printf("Could not create pipe...\n");
    exit(1);
  }

  printf("Now going to fork...\n");
  pid_t pid = fork();
  if(pid == 0) {
    printf("Close read fd...\n");
    close(fd[0]);
    printf("Duplicate write fd...\n");
    if(dup2(fd[1], 3) !=3 ) {
      printf("Could not duplicate fd to nr 3, going to DIE!\n");
      exit(1);
    }
    if(fd[1] != 3) {
      printf("Closing old fd ...\n");
      close(fd[1]);
    }
    
    printf("Hello world! What a nice place to live! Let's talk with daddy!\n");
    dprintf(3, "Hi dad, how are you doing?\n");
    printf("Oje, it's time to die. What a short live...\n");
    exit(0);
  } else if (pid > 0) {
    printf("Close write fd...\n");
    close(fd[1]);
    printf("Oh, I became father of %x. That feels great! Let's wait, what he has to say...\n", pid);
    char buff[256];
    int num = read(fd[0], (void *) buff, 255);
    buff[num] = '\0';
    printf("Read %d bytes: %s\n", num, buff);

    printf("Closing the pipe...\n");
    close(fd[0]);
    close(fd[1]);

    printf("Now return...\n");
  } else {
    printf("There was an error while forking! Am I impotent?\n");
  }
}
void pipe4() {
  // Create a pipe, fork, child duplicates file descriptor, exec -> echo to new fd.
  int fd[2];
  if( pipe(fd) != 0) {
    printf("Could not create pipe...\n");
    exit(1);
  }

  printf("Now going to fork...\n");
  pid_t pid = fork();
  if(pid == 0) {
    printf("Close read fd...\n");
    close(fd[0]);
    printf("Duplicate write fd...\n");
    if(dup2(fd[1], 3) !=3 ) {
      printf("Could not duplicate fd to nr 3, going to DIE!\n");
      exit(1);
    }
    if(fd[1] != 3) {
      printf("Closing old fd ...\n");
      close(fd[1]);
    }
    printf("Hello world! What a nice place to live! Let's exec and transform to shell!\n");
    
    //dprintf(3, "Hi dad, how are you doing?\n");
    const char* args[] = {
      "/bin/env", "bash",  "-c", "echo 'Hi dad, how are you doing?' >&3", NULL};
    execv("/bin/env", (char **) args); // Stupid function does not like chonst
                                       // Why?
    printf("Exec Error...\n");
    exit(1);
  } else if (pid > 0) {
    printf("Close write fd...\n");
    close(fd[1]);
    printf("Oh, I became father of %x. That feels great! Let's wait, what he has to say...\n", pid);
    char buff[256];
    int num = read(fd[0], (void *) buff, 255);
    buff[num] = '\0';
    printf("Read %d bytes: %s\n", num, buff);

    printf("Closing the pipe...\n");
    close(fd[0]);
    close(fd[1]);

    printf("Now return...\n");
  } else {
    printf("There was an error while forking! Am I impotent?\n");
  }
}
void pipe5() {
  // Create a pipe, fork, child duplicates file descriptor, exec a xterm -> echo to new fd.
  int fd[2];
  if( pipe(fd) != 0) {
    printf("Could not create pipe...\n");
    exit(1);
  }

  printf("Now going to fork...\n");
  pid_t pid = fork();
  if(pid == 0) {
    printf("Close read fd...\n");
    close(fd[0]);
    printf("Duplicate write fd...\n");
    if(dup2(fd[1], 3) !=3 ) {
      printf("Could not duplicate fd to nr 3, going to DIE!\n");
      exit(1);
    }
    if(fd[1] != 3) {
      printf("Closing old fd ...\n");
      close(fd[1]);
    }
    printf("Hello world! What a nice place to live! Let's exec and transform to shell!\n");
    
    //dprintf(3, "Hi dad, how are you doing?\n");
    const char* args[] = {
      "/bin/env", "xterm", "-hold", "-e", "echo 'Hi dad, how are you doing?' >&3", NULL};
    execv("/bin/env", (char **) args); // Stupid function does not like chonst
                                       // Why?
    printf("Exec Error...\n");
    exit(1);
  } else if (pid > 0) {
    printf("Close write fd...\n");
    close(fd[1]);
    printf("Oh, I became father of %x. That feels great! Let's wait, what he has to say...\n", pid);
    char buff[256];
    int num = read(fd[0], (void *) buff, 255);
    buff[num] = '\0';
    printf("Read %d bytes: %s\n", num, buff);

    printf("Closing the pipe...\n");
    close(fd[0]);
    close(fd[1]);

    printf("Now return...\n");
  } else {
    printf("There was an error while forking! Am I impotent?\n");
  }
}
void pipe6() {
  // Create a pipe, fork, child duplicates file descriptor, exec a xterm -> run simulated machine helper.
  int fd[2];
  if( pipe(fd) != 0) {
    printf("Could not create pipe...\n");
    exit(1);
  }

  printf("Now going to fork...\n");
  pid_t pid = fork();
  if(pid == 0) {
    printf("Close read fd...\n");
    close(fd[0]);
    printf("Duplicate write fd...\n");
    if(dup2(fd[1], 3) !=3 ) {
      printf("Could not duplicate fd to nr 3, going to DIE!\n");
      exit(1);
    }
    if(fd[1] != 3) {
      printf("Closing old fd ...\n");
      close(fd[1]);
    }
    printf("Hello world! What a nice place to live! Let's exec and transform to shell!\n");
    
    //dprintf(3, "Hi dad, how are you doing?\n");
    const char* args[] = {
      "/bin/env", "xterm", "-hold", "-e", "./simulatedMachineHelper", NULL};
    execv("/bin/env", (char **) args); // Stupid function does not like chonst
                                       // Why?
    printf("Exec Error...\n");
    exit(1);
  } else if (pid > 0) {
    printf("Close write fd...\n");
    close(fd[1]);
    printf("Oh, I became father of %x. That feels great! Let's wait, what he has to say...\n", pid);
    char buff[256];
    int num = read(fd[0], (void *) buff, 255);
    buff[num] = '\0';
    printf("Read %d bytes: %s\n", num, buff);

    printf("Closing the pipe...\n");
    close(fd[0]);
    close(fd[1]);

    printf("Now return...\n");
  } else {
    printf("There was an error while forking! Am I impotent?\n");
  }
}
  

int main() {
  //pipe1();
  pipe6();
  return 0;
}


/*
  // 0 -> input -> father keeps it
  // 1 -> output -> child keeps it and makes it nr 3

  // now let's fork
  pid_t pid = fork();
  if (pid == 0) { // Child
    // Fortunately dup2 does pretty much what we want!
    if(dup2(fd[1], 3) !=3 ) {
      cerr << "Error while duplicating pipe descriptor" <<endl;
    }
    // Now we want the old descriptor to close on exec:
    if(fd[1] != 3) {
      fcntl(fd[1], F_SETFD, FD_CLOEXEC);
    }
    fcntl(fd[0], F_SETFD, FD_CLOEXEC);
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
      while(not (num = read(fd[0], (void *) buff, 255))) {
        cout << " waiting for child process ..." << endl;
        usleep(500000);
      }
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
    



*/
  

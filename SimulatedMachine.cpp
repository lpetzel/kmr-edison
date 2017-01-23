#include "SimulatedMachine.h"
#include <iomanip>

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

template<class T> void SimulatedMachine<T>::getRegister(int i, int) {
  std::lock_guard<std::mutex> guard(lock_);
  int nr = T::getNrRegisters();
  bool report = (report_mode_ & REPORT_UPDATE) and isInitialized();
  if( report and
      (T::registers_.at(i) != registers_sps_.at(i))) {
    getOStream() << "updated register " << i << std::endl;
  }
  T::registers_[i] = registers_sps_.at(i);
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

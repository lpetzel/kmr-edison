#include "Machine.h"
#include <stdexcept>
#include <iostream>

Machine::Machine(): offset_(0) {}

Machine::Machine(int nr_registers): registers_(nr_registers), offset_(0), mode_((nr_registers + 3)/4) {
  for (int i = 0; i < nr_registers; i++) {
    registers_.push_back(0); // fill all with 0
    mode_.push_back(0xFF);   // fill all with read+write (io)
  }
}

int Machine::getRegister(int nr) const {
  return registers_.at(nr);
}


// Immediately get the register from plc.
// No checks are done
// value is cached for later access
int Machine::getRegister(int nr, int) {
  std::cout << "Not implemented yet: Machine::updateRegister(int, int)\n";
  if ( nr < 0 or nr > getNrRegisters())
    throw std::out_of_range("The register is not mapped by this machine.");
  modbus_read_registers( connection_, offset_ + nr, 1, (unsigned short *) registers_.data() + nr);
  /*if (not checkState()) {
    repairBrokenState();
  }*/
  return getRegister(nr);
}
  

void Machine::setRegister(int nr, signed int value) {
  if (isWritable(nr)) {
    registers_[nr] = value;
  } else {
    throw std::invalid_argument("The specified register is not writable");
  }
}

void Machine::setRegister(int nr, signed int value, int) {
  setRegister(nr, value);
  std::cout << "Not implemented yet: Machine::setRegister(int, int, int)\n";
}


void Machine::repairBrokenState() {
  if(not checkState()) {
      throw std::logic_error( "Could not repair broken state" );
  }
}

// update all registers, which can be updated (r or rw)
// the modbus library allows a range of values to be updated
void Machine::updateRegisters() {
  int nr_reg = getNrRegisters();
  int i_from = 0;
  for( int i = 0; i < nr_reg; ++i) {
    if (!isReadable( (unsigned short) i)) {
      if ( i != i_from ) { // They are equal, if last and current element were not readable
        modbus_read_registers( connection_, offset_ + i_from, i - i_from, (unsigned short*) registers_.data() + i_from);
      }
      i_from = i + 1;
    }
  }
  if ( nr_reg != i_from ) { // They differ, if the last elements were readable
    modbus_read_registers( connection_, offset_ + i_from, nr_reg - i_from, (unsigned short*) registers_.data() + i_from);
  }
  if (not checkState()) {
    repairBrokenState();
  }
}

// write out all registers to plc
void Machine::pushRegisters() {
  int nr_reg = getNrRegisters();
  int i_from = 0;
  for( int i = 0; i < nr_reg; ++i) {
    if (!isWritable( (unsigned short) i)) {
      if ( i != i_from ) { // They are equal, if last and current element were not readable
        // TODO check syntax
        modbus_write_registers( connection_, offset_ + i_from, i - i_from, (unsigned short*) registers_.data() + i_from);
      }
      i_from = i + 1;
    }
  }
  if ( nr_reg != i_from ) { // They differ, if the last elements were readable
    modbus_read_registers( connection_, offset_ + i_from, nr_reg - i_from, (unsigned short*) registers_.data() + i_from);
  }
}

void Machine::connectSPS(std::string ip, unsigned short port) {
  connection_ = modbus_new_tcp(ip.c_str(), port);
  modbus_connect(connection_);
  updateRegisters();
}

Machine::~Machine() {
  modbus_close(connection_);
  modbus_free(connection_);
}


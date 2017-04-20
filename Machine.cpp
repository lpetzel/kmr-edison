#include "Machine.h"
#include "MPSIoMapping.h"
#include <stdexcept>

using namespace std;

Machine::Machine(): in_registers_(4), out_registers_(4), connection_(nullptr) {
  for (int i = 0; i < 4; ++i) {
    in_registers_.push_back(0);
    out_registers_.push_back(0);
  }
}


void Machine::sendCommand(unsigned short command, unsigned short payload1, unsigned short payload2, unsigned char status) {
  out_registers_[0] = command;
  out_registers_[1] = payload1;
  out_registers_[2] = payload2;
  out_registers_[3] = status;
  //std::lock_guard<std::mutex> g(lock_);
  waitForReady();
  pushRegisters();
}

void Machine::waitForReady() {
  do {
    // TODO: maybe timing control?
    // TODO: maybe different approach for exit possible?
    updateRegisters();
    if (in_registers_.at(4) & STATUS_BUISY) {
      out_registers_[3] &= ~ STATUS_BUISY;
      pushRegisters();
    }
  } while (! (in_registers_.at(4) & (STATUS_READY | STATUS_ERR)));
}


// TODO: do error handeling here.
//       eg throw error
//       and maybe add error to an error list for protobuf messages
void Machine::updateRegisters() {
  modbus_read_registers( connection_, 0, 4, (unsigned short*) in_registers_.data());
}

// write out all registers to plc
void Machine::pushRegisters() {
  modbus_write_registers( connection_, 0, 4, (unsigned short*) out_registers_.data());
}

void Machine::connectPLC(const std::string& ip, unsigned short port) {
  connection_ = modbus_new_tcp(ip.c_str(), port);
  modbus_connect(connection_);
  updateRegisters();
}

Machine::~Machine() {
  modbus_close(connection_);
  modbus_free(connection_);
}

  

void Machine::setLight(unsigned short color, unsigned short state, unsigned short time) {
  switch (color) {
    case LIGHT_RESET_CMD:
    case LIGHT_RED_CMD:
    case LIGHT_YELLOW_CMD:
    case LIGHT_GREEN_CMD:
      break;
    default: throw invalid_argument("Illegal color! See MPSIoMapping.h for choices.");
  }
  sendCommand( color, state, time);
}


void Machine::resetLight() {
  setLight(LIGHT_RESET_CMD, 0);
}

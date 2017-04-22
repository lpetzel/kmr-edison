#include "BaseStation.h"
#include "MPSIoMapping.h"
#include "MachineProtoServer.h"
#include <iostream>

using namespace std;

BaseStation::BaseStation() {}
BaseStation::~BaseStation() {}

void BaseStation::getBase(unsigned short color) {
  getBase( color, DIR_OUT);
}

void BaseStation::getBase(unsigned short color, unsigned short side) {
  lock_guard<mutex> g(lock_);
  sendCommand(BASE_STATION_CMD + GET_BASE_CMD, color);
  sendCommand(BASE_STATION_CMD + MOVE_BAND_CMD, side);
  waitForReady();
}

// Need information on how to access this
bool BaseStation::baseReady() {
  cout << "Not implemented yet!" << endl;
  return true;
}

// Need information on how to access this
bool BaseStation::isEmpty() {
  cout << "Not implemented yet!" << endl;
  return false;
}

void BaseStation::reset() {
  sendCommand(BASE_STATION_CMD | RESET_CMD);
}

void BaseStation::identify() {
  sendCommand(SET_TYPE_CMD, TYPE_BS);
}

#define CASE(type) } else if (dynamic_cast<type *> (&m)) {auto mc = dynamic_cast<type *> (&m);
#define ACK auto msg = make_shared<MPSFinished>();\
  msg->set_id(mc->id()); \
  s.send_to_all(msg)

void BaseStation::handleProtobufMsg(google::protobuf::Message& m, MachineProtoServer& s) {
  if (0) {
  CASE(MoveConveyorBelt)
    sendCommand(MOVE_BAND_CMD + BASE_STATION_CMD, mc->stop_sensor(), 0, // CMD, data1, data2
      TIMEOUT_BAND); // Timeout
    ACK;
    // TODO: send MPSProductRetrived, when Product is retrieved.
    // Unfortunately I don't have a msp message for that yet.
    // Furthermore, this should be done in background. How?
    // Idea: Machine gets a member eg list<ProtobufServers*> or so.
    // On updateRegister() -> check a bit.
    // OR:
    // Background thread, which can be activated and deactivated.
    // This thread fights for the mutex and, on a periodical base,
    // checks, weather the product was retrieved.
  CASE(BSPushBase)
    sendCommand(GET_BASE_CMD + BASE_STATION_CMD, mc->slot() + 1);
    ACK;
  } else {
    Machine::handleProtobufMsg(m, s);
  }
}

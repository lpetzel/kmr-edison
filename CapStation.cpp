#include "CapStation.h"
#include "MPSIoMapping.h"
#include "protobuf/proto.pb.h"
#include "MachineProtoServer.h"
#include <iostream>

using namespace std;

CapStation::CapStation() {}
CapStation::~CapStation() {}

void CapStation::reset() {
  sendCommand(CAP_STATION_CMD + RESET_CMD);
}

void CapStation::retrieveCap() {
  sendCommand(CAP_ACTION_CMD + CAP_STATION_CMD, CAP_RETRIEVE);
}

void CapStation::mountCap() {
  sendCommand(CAP_ACTION_CMD + CAP_STATION_CMD, CAP_MOUNT);
}

void CapStation::identify() {
  sendCommand(SET_TYPE_CMD, TYPE_CS);
}

#define CASE(type) } else if (dynamic_cast<type *> (&m)) {auto mc = dynamic_cast<type *> (&m);
#define ACK auto msg = make_shared<MPSFinished>();\
  msg->set_id(mc->id()); \
  s.send_to_all(msg)

void CapStation::handleProtobufMsg(google::protobuf::Message& m, MachineProtoServer& s) {
  if (0) {
  CASE(MoveConveyorBelt)
    sendCommand(MOVE_BAND_CMD + CAP_STATION_CMD, mc->stop_sensor(), 0, // CMD, data1, data2
      TIMEOUT_BAND);
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
  CASE(CSTask)
    sendCommand(CAP_ACTION_CMD + CAP_STATION_CMD, mc->operation());
    ACK;
  } else {
    Machine::handleProtobufMsg(m, s);
  }
}

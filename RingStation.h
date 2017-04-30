#pragma once

#include "Machine.h"
#include "protobuf/MachineInstructions.pb.h"

class RingStation: public Machine {
  public:
    RingStation();
    virtual ~RingStation();

    // Send command to get a ring
    // void getRing();
    // Check, if the cap is ready for take away
    bool ringReady();
    // Handle protobuf message (from refbox)
    void handleProtobufMsg(google::protobuf::Message& m, MachineProtoServer& s);

    virtual void identify();
    void reset();
};

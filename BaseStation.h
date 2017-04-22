#pragma once

#include "Machine.h"
#include "protobuf/proto.pb.h"

class BaseStation: public Machine {
  public:
    BaseStation();
    virtual ~BaseStation();

    // Send command to get a cap of given color
    // and on given side
    void getBase(unsigned short slot, unsigned short side);
    void getBase(unsigned short color);
    // Check, if the cap is ready for take away
    bool baseReady();
    // Check, if the last cap was taken -> new order can be processed
    bool isEmpty();
    // Handle protobuf message (from refbox)
    void handleProtobufMsg(google::protobuf::Message& m, MachineProtoServer& s);

    virtual void identify();
    void reset();
};

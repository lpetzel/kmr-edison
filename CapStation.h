#pragma once

#include "Machine.h"

class CapStation: public Machine {
  public:
    CapStation();
    void retrieveCap();
    void mountCap();
    virtual ~CapStation();

    // Handle protobuf message (from refbox)
    void handleProtobufMsg(google::protobuf::Message& m, MachineProtoServer& s);
    virtual void identify();
    void reset();
};

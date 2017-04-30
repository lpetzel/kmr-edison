#pragma once

#include "Machine.h"
#include "protobuf/MachineInstructions.pb.h"

class StorageStation: public Machine {
  public:
    StorageStation();
    virtual ~StorageStation();

    // Send command to deliver a product
    // slot is between 1 and 3
    void getProduct(int slot);

    // Handle protobuf message (from refbox)
    void handleProtobufMsg(google::protobuf::Message& m, MachineProtoServer& s);

    virtual void identify();
    void reset();
};

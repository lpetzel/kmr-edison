// Storage Station
// The storage station can be used to store products.
// It will not work as intended, because protocols do not match yet.
#pragma once

#include "Machine.h"
#include "protobuf/MachineInstructions.pb.h"

class StorageStation: public Machine {
  public:
    StorageStation();
    virtual ~StorageStation();

    // Send command to deliver a product
    // slot is between 1 and 3
    // deprecated
    void getProduct(int slot);

    // Handle protobuf message (from refbox)
    void handleProtobufMsg(google::protobuf::Message& m, MachineProtoServer& s);

    // identify: tell the PLC, which machine it is controlling
    // reset: send reset command
    virtual void identify();
    virtual void reset();
};

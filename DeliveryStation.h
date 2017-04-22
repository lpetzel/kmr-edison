#pragma once

#include "Machine.h"
#include "protobuf/proto.pb.h"

class DeliveryStation: public Machine {
  public:
    DeliveryStation();
    virtual ~DeliveryStation();

    // Send command to deliver a product
    // slot is between 1 and 3
    void deliverProduct(int slot);

    // Handle protobuf message (from refbox)
    void handleProtobufMsg(google::protobuf::Message& m, MachineProtoServer& s);

    virtual void identify();
    void reset();
};

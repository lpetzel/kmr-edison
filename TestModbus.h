// TestModbus
// is a test class for some manual modbus testing without protobuf being
// involved.
// The class uses partly out-dated protocol specifications and deprecated
// methods. It is therfore not reliable.
#pragma once

class BaseStation;
class RingStation;
class CapStation;
class DeliveryStation;
class StorageStation;

class TestModbus {
  public:
    static void run(BaseStation*);
    static void run(RingStation*);
    static void run(CapStation*);
    static void run(DeliveryStation*);
    static void run(StorageStation*);
};

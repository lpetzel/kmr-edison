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

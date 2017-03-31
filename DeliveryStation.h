#pragma once

#include "Machine.h"

class DeliveryStation: public Machine {
  public:
    DeliveryStation();
    virtual ~DeliveryStation();

    // Send command to deliver a product
    // slot is between 1 and 3
    void deliverProduct(int slot);

    void reset();
};

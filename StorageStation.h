#pragma once

#include "Machine.h"

class StorageStation: public Machine {
  public:
    StorageStation();
    virtual ~StorageStation();

    // Send command to deliver a product
    // slot is between 1 and 3
    void getProduct(int slot);

    void reset();
};

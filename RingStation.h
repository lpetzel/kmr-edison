#pragma once

#include "Machine.h"

class RingStation: public Machine {
  public:
    RingStation();
    virtual ~RingStation();

    // Send command to get a ring
    void getRing();
    // Check, if the cap is ready for take away
    bool ringReady();

    void reset();
};

#pragma once

#include "Machine.h"

class BaseStation: public Machine {
  public:
    BaseStation();
    virtual ~BaseStation();

    // Send command to get a cap of given color
    // and on given side
    void getCap(unsigned short color, unsigned short side);
    void getCap(unsigned short color);
    // Check, if the cap is ready for take away
    bool capReady();
    // Check, if the last cap was taken -> new order can be processed
    bool isEmpty();

    void reset();
};

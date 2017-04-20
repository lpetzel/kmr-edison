#pragma once

#include "Machine.h"

class CapStation: public Machine {
  public:
    CapStation();
    void retrieveCap();
    void mountCap();
    virtual ~CapStation();

    void reset();
};

#pragma once

#include "Machine.h"

class CapStation: public Machine {
  public:
    CapStation();
    virtual ~CapStation();

    void reset();
};

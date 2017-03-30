#pragma once

#include "Machine.h"

class BaseStation: public Machine {
  public:
    BaseStation();
    virtual ~BaseStation();
    virtual bool checkModbusRequest const();
    virtual bool checkState const();
    virtual void repairBrokenState();
    virtual void printState(std::ostream&) const;

    // Send command to get a cap of given color
    // and on given side
    virtual void getCap(unsigned short color, unsigned short side);
    // Check, if the cap is ready for take away
    virtual bool capReady();
    // Check, if the last cap was taken -> new order can be processed
    virtual bool isEmpty();
    virtual void clearRegister();
    virtual void deliverProduct();
    virtual int updateState();
}

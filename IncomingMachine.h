#pragma once

#include "Machine.h"

class IncomingMachine: public Machine {
  public:
    IncomingMachine();
    virtual ~IncomingMachine();
    virtual bool checkModbusRequest const();
    virtual bool checkState const();
    virtual void repairBrokenState();
    virtual void printState(std::ostream&) const;

    // Send command to get a cap of given color
    // and on given side
    virtual void getCap(unsigned short color, unsigned short side);
    virtual bool capReady();
    virtual bool isEmpty();
    virtual void setLight(int color, int state);
    virtual void clearRegister();
    virtual void deliverProduct();
    virtual int updateState();
}

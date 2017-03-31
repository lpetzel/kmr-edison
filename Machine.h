#pragma once
#include <vector>
#include <string>
#include <modbus.h>
//#include <ostream>


#include <mutex>


class Machine {
  public:
    Machine();

    virtual ~Machine();

    // 1 = BUISY
    void sendCommand( unsigned short command, unsigned int payload = 0, unsigned char status = 1);

    // Create a modbus connection to machine
    // virtual -> some child classes might only simulate the sps
    void connectPLC(const std::string& ip, unsigned short port);

    // void setLight(int color, int state);

    virtual void reset() = 0;

    std::mutex lock_;

  protected:
    void waitForReady();
    void updateRegisters();
    void pushRegisters();
    // Input registers
    std::vector<unsigned short> in_registers_;
    // Output registers
    std::vector<unsigned short> out_registers_;
    modbus_t* connection_;
};
    




#pragma once
#include <vector>
#include <string>
#include <modbus.h>



class Machine {
  public:
    Machine();
    // Initialize machine with number of registers, all io
    Machine(int nr_registers);

    virtual ~Machine();

    // read cached register values (per default)
    // The version with 3 registers will get this
    // value directly from the machine. It will also
    // update the cached value
    virtual int getRegister(int nr) const;
    virtual int getRegister(int nr, int);

    // write register value to cache (per default)
    // virtual -> some child classes might do some checking
    // The variant with 3 arguments will send this one register
    // immediately to the machine
    virtual void setRegister(int nr, signed int val);
    virtual void setRegister(int nr, signed int val, int);
    
    // Get number or registers
    int getNrRegisters() const {return registers_.size();}
    
    // Check for errors (default -> true -> no errors)
    // Check for errors in request
    // Before sending the registers to machine, this will check, if the
    // requested changes are okay
    virtual bool checkModbusRequest() const {return true;}
    // Check for errors (default -> true -> no errors)
    // This function will be called after the registers are read from the PLC
    virtual bool checkState() const {return true;}
    // Try to repair a broken state (default: Throw error)
    // checkState must return true after a call to this method
    virtual void repairBrokenState();

    // read all in and io registers from the machine
    virtual void updateRegisters();
    // write all out and io registers to the machine
    virtual void pushRegisters();

    // Create a modbus connection to machine
    // virtual -> some child classes might only simulate the sps
    virtual void connectSPS(std::string ip, unsigned short port);
  protected:
    // All in/out/io-registers
    std::vector<signed short> registers_;
    // Add this offset when communicating with machine
    unsigned short offset_;
    // Each register has two bits:
    // lower bit: update from machine
    // higher bit: write to machine
    // addresses per entry
    std::vector<unsigned char> mode_;
    modbus_t* connection_;

    // return true if register i gets updated (in- or io-mode)
    bool isReadable(unsigned short addr) const {
      unsigned char mask = (addr & 0b11) << 1;
      addr = addr >> 2;
      mask = 1 << mask; // 1 = read
      return registers_.at(addr) & mask;
    }
    // returns true, if the register gets pushed to the machine (out- or io-mode)
    bool isWritable(unsigned short addr) const {
      unsigned char mask = (addr & 0b11) << 1;
      addr = addr >> 2;
      mask = 2 << mask; // 2 = write
      return registers_.at(addr) & mask;
    }
};
    




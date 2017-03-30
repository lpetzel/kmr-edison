#pragma once
#include <vector>
#include <string>
#include <modbus.h>
#include <ostream>

// Question: create a mutex lock?
// Most operations seem to be anyway pretty save...
// Maybe lock only for write access?
// But then why at all?
// #include <mutex>


class Machine {
  public:
    Machine();
    // Initialize machine with number of registers, all io
    explicit Machine(int nr_input_reg, int nr_output_reg);

    virtual ~Machine();

    // read cached register values (per default)
    int getInputRegister(int nr) const;
    int getOutputRegister(int nr) const;

    // write register value to cache (per default)
    void setRegister(int nr, signed int val);

    // Sets only a single bit.
    // 
    void setPin(int pin, bool value = true);
    
    // Get number or registers
    int getNrRegisters() const {return registers_.size();}
    
    // Check for errors (default -> true -> no errors)
    // This function will be called after the registers are read from the PLC
    virtual bool checkState() const {return true;}
    // Try to repair a broken state (default: Throw error)
    // checkState must return true after a call to this method
    virtual void repairBrokenState();

    // read all registers from the machine
    virtual void updateRegisters();
    // write all out registers to the machine
    virtual void pushRegisters();

    // Create a modbus connection to machine
    // virtual -> some child classes might only simulate the sps
    virtual void connectPLC(std::string ip, unsigned short port);

    // Print state in some form
    virtual void printState(std::ostream&) const;

    //virtual void setLight(int color, int state);
    
    // Each sub class has to know its id in order to send the identification to the PLC
    virtual unsigned short getMachineIdentification() const=0;

    // std::mutex lock_;
  protected:

    // Depending on the machine, set the identification code
    // for the SPS program
    // Currently, that is at INPUT[10]:
    // 1: base station
    // 2: ring station
    // 3: cap station
    // 4: deliver station
    // 5: SS?
    void sendMachineIdentification(unsigned short id);


    // Input registers
    std::vector<signed short> in_registers_;
    // Output registers
    std::vector<signed short> out_registers_;
    modbus_t* connection_;
    unsigned int state_;

};
    




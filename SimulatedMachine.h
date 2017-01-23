#pragma once

#include "Machine.h"
#include <istream>
// If Machine has already a mutex member, the mutex
// of the base class can be used. This is an open design question.
#include <mutex>

#define REPORT_PUSH 1
#define REPORT_UPDATE 2
//#define REPORT_UPDATE 2


template<class T>
class SimulatedMachine: public T {
  public:
    SimulatedMachine();
    explicit SimulatedMachine(int nr_registers);
    virtual ~SimulatedMachine();

    // Do nothing
    void connectSPS(const std::string&, unsigned short);
    // Create Copy of write registers
    // And maybe inform user about the changes
    void pushRegisters();
    // Udate registers with values from simulated machine
    void updateRegisters();
    // Update only one register
    void getRegister(int, int);
    
    // Show differences machine/registers
    void showDiff() const;
    virtual void showState() const;

    // Get in-/output streams for alternative terminal session
    std::ostream& getOStream() const;
    std::istream& getIStream() const;

    // Initialize a TTY for access to machine
    // Virtual, so that a sub class can initialize
    // eg over some remote tty
    // after calling this, the in- and output streams
    // can be used
    virtual void initializeTTY();

    // opens an iterface to the user
    // virtual, so that sub classes can inform the user
    // in custom ways
    virtual void openUserInterface();

    // To run as separate thread:
    // Take user input and modify machine content on demand
    virtual void run();

    // Returns true, if the TTY is already initialized
    // So that getIStream and getOStream will work
    bool isInitialized() const;

    mutable std::mutex lock_;
    unsigned int report_mode_;
  protected:
    
    std::istream* in_;
    std::ostream* out_;
    std::vector<signed short> registers_sps_;

};


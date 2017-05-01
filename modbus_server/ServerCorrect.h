// This file is for testing. It just imitates a very basic PLC.
// Currently all commands take 5 requests to finish, so really basic.
#pragma once

/*
 * Just a simple modbus slave, which takes commands,
 * and after 5 requests writes back the ready bit.
 *
 * This program is for testing without an actual machine.
 * Has to be started with root access, if port is 502.
 */

#include <modbus.h>
#include <string>

class ServerCorrect {
  public:
    // Server will listen on ip and port. Ip can be empty -> listen on all
    // addresses.
    // Port is 502 -> root access. If higher then 1024 or so, not root access
    // is needed.
    std::string ip_;
    unsigned short port_;
    //unsigned short state_[4];

    // This contains the modbus register
    // reg 0 -> command
    // reg 1 -> data1
    // reg 2 -> data2
    // reg 3 -> state
    modbus_mapping_t *map_;

    // Constructer
    ServerCorrect(const std::string& ip, unsigned short port);

    ~ServerCorrect();

    // Let the user communicate with me.
    void Run();
};

// Prints the command name of the given command number.
// Prefix is the machine type or ANY.
// eg ANY_SET_GREEN_LIGHT.
const char* commandName(unsigned short cmd);

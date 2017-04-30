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
    std::string ip_;
    unsigned short port_;
    unsigned short state_[4];
    modbus_mapping_t *map_;

    ServerCorrect(const std::string& ip, unsigned short port);

    ~ServerCorrect();

    void Run();
};

const char* commandName(unsigned short cmd);

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include "../MPSIoMapping.h"
#include "ServerCorrect.h"

using namespace std;

ServerCorrect::ServerCorrect(const std::string& ip, unsigned short port)
    : ip_(ip), port_(port) {
  map_ = modbus_mapping_new(0,0,4,0);
  // free(map_->tab_registers);
  // map_->tab_registers = map_->tab_input_registers;
}

ServerCorrect::~ServerCorrect() {
  // map_->tab_registers = nullptr;
  modbus_mapping_free(map_);
}


void ServerCorrect::Run() {
  assert(map_);
  modbus_t *ctx = modbus_new_tcp(ip_.c_str(), port_);
  assert(ctx);
  // Allow exaclty 1 conncetion
  int conn = modbus_tcp_listen(ctx, 1);
  modbus_tcp_accept(ctx, &conn);
  cout << "Connection established" << endl;
  int rc;
  bool bsy_old;
  int count = 0;
  // Probably need way less than 100
  unsigned char query[100];
  do {
    rc = modbus_receive(ctx, query);
    if (rc > 0) {
      modbus_reply(ctx,query,rc,map_);
      // cout << "(" << map_->tab_registers[0] << ", " << map_->tab_registers[3] << ")" << endl;
      // cout << " <- (" << map_->tab_input_registers[0]  << ", " << map_->tab_input_registers[3] << ")" << endl;
      // map_->tab_registers[0] = map_->tab_input_registers[0];
      // map_->tab_registers[1] = map_->tab_input_registers[1];
      // map_->tab_registers[2] = map_->tab_input_registers[2];
      // map_->tab_registers[3] = map_->tab_input_registers[3];
      if((map_->tab_registers[3] & STATUS_BUISY) && (! bsy_old)) {
        cout << "Start Command " << commandName(map_->tab_registers[0]) << "(" << map_->tab_registers[0] << ", "
          << map_->tab_registers[1] << ", " << map_->tab_registers[2] << ")" <<endl;
        count = 0;
      } else if (! (map_->tab_registers[3] & STATUS_READY)) {
        // Command is running, after 5 packages it is finished
        ++count;
        if (count > 5) {
          cout << "Command " << commandName(map_->tab_registers[0]) << " finished (setting READY)" << endl;
          map_->tab_registers[3] |= STATUS_READY;
        }
      }
      bsy_old = map_->tab_registers[3] & STATUS_BUISY;
    }
  } while (rc != -1);
  cout << "Connection ended" << endl;
  if (conn != -1) {
    cout << "Close FD" << endl;
    close(conn);
  }
  cout << "Close connection" << endl;
  modbus_close(ctx);
  modbus_free(ctx);
}


int main(int argc, char** argv) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " <ip> <port>" << endl;
    return 1;
  }
  unsigned short port = (unsigned short) atoi(argv[2]);
  cout << "Creating Server object for ip = " << argv[1] << " (" << port <<")" << endl;
  ServerCorrect server (argv[1], port);
  cout << "Start server" << endl;
  server.Run();
  cout << "Bye" << endl;
  return 0;
}


#define REGISTER_CMD_STR(station, command, prefix, string) case (station + command): return prefix "_" string;
#define REGISTER_CMD(station, command) case (station + command): return #station "_" #command;

#define BS BASE_STATION_CMD
#define RS RING_STATION_CMD
#define CS CAP_STATION_CMD
#define DS DELIVERY_STATION_CMD
#define SS STORAGE_STATION_CMD
#define ANY 0

#define REGISTER_ALL(command) \
  REGISTER_CMD_STR(BS, command, "BS", #command) \
  REGISTER_CMD_STR(RS, command, "RS", #command) \
  REGISTER_CMD_STR(CS, command, "CS", #command) \
  REGISTER_CMD_STR(SS, command, "SS", #command) \
  REGISTER_CMD_STR(DS, command, "DS", #command)

const char* commandName(unsigned short cmd) {
  switch (cmd) {
    REGISTER_CMD(ANY, SET_TYPE_CMD)
    REGISTER_CMD(ANY, LIGHT_RESET_CMD)
    REGISTER_CMD(ANY, LIGHT_RED_CMD)
    REGISTER_CMD(ANY, LIGHT_YELLOW_CMD)
    REGISTER_CMD(ANY, LIGHT_GREEN_CMD)
    REGISTER_ALL(RESET_CMD)
    REGISTER_ALL(MOVE_BAND_CMD)
    REGISTER_CMD(BS, GET_BASE_CMD)
    REGISTER_CMD(RS, WAIT_FOR_BASES_CMD)
    REGISTER_CMD(RS, MOUNT_RING_CMD)
    REGISTER_CMD(CS, CAP_ACTION_CMD)
    REGISTER_CMD(DS, DELIVER_CMD)
    REGISTER_CMD(SS, GET_F_PRODUCT_CMD)
    default: return "UNKNOWN";
  }
}



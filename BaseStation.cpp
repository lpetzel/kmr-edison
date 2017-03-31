#include "BaseStation.h"
#include "MPSIoMapping.h"
#include <iostream>

using namespace std;

BaseStation::BaseStation() {}
BaseStation::~BaseStation() {}

void BaseStation::getCap(unsigned short color) {
  getCap( color, DIR_OUT);
}

void BaseStation::getCap(unsigned short color, unsigned short side) {
  lock_guard<mutex> g(lock_);
  sendCommand(BASE_STATION_CMD | GET_BASE_CMD, color);
  sendCommand(BASE_STATION_CMD | MOVE_BASE2END_CMD, side);
  waitForReady();
}

// Need information on how to access this
bool BaseStation::capReady() {
  cout << "Not implemented yet!" << endl;
  return true;
}

// Need information on how to access this
bool BaseStation::isEmpty() {
  cout << "Not implemented yet!" << endl;
  return false;
}

void BaseStation::reset() {
  sendCommand(BASE_STATION_CMD | RESET_CMD);
}
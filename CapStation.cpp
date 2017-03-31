#include "CapStation.h"
#include "MPSIoMapping.h"
#include <iostream>

using namespace std;

CapStation::CapStation() {}
CapStation::~CapStation() {}

void CapStation::reset() {
  sendCommand(CAP_STATION_CMD | RESET_CMD);
}

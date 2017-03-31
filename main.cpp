#include "TestModbus.h"
#include "BaseStation.h"
#include <iostream>

using namespace std;
int main( int argc, char** argv ) {
  if (argc != 2) {
    cout << "Usage <cmd> ip-address" << endl;
    return 1;
  }
  BaseStation s;
  s.connectPLC(argv[1], 502);
  cout << "Connection to plc established." << endl;
  TestModbus::run(&s);
  cout << "Going to quit" << endl;
  return 0;
}
  
  

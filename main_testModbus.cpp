#include "TestModbus.h"
#include "BaseStation.h"
#include <iostream>
#include <cstdlib>

using namespace std;
int main( int argc, char** argv ) {
  if (argc != 3) {
    cout << "Usage <cmd> ip-address port" << endl;
    return 1;
  }
  unsigned short port = (unsigned short) atoi(argv[2]);
  BaseStation s;
  s.connectPLC(argv[1], port);
  cout << "Connection to plc established." << endl;
  TestModbus::run(&s);
  cout << "Going to quit" << endl;
  return 0;
}
  
  

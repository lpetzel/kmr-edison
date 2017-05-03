#include "MachineProtoServer.h"
#include "BaseStation.h"
#include "StorageStation.h"
#include "CapStation.h"
#include "DeliveryStation.h"
#include "RingStation.h"
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <pthread.h>
#include <algorithm>
#include <locale>

using namespace std;

int main(int argc, char** argv) {
  if (argc != 5) {
    cerr << "Usage: " << argv[0] << " <Machine Type> <Machine ip> <Machine port> <Protobuf port>" << endl;
    return 1;
  }
  string type = argv[1];
  unsigned short port_pb = (unsigned short) atoi(argv[4]);
  unsigned short port_plc = (unsigned short) atoi(argv[3]);

  transform(type.begin(), type.end(), type.begin(), (int (*) (int)) std::tolower);

  Machine* m;

  if(type == "bs") {
    m = new BaseStation();
  } else if (type == "rs") {
    m = new RingStation();
  } else if (type == "cs") {
    m = new CapStation();
  } else if (type == "ds") {
    m = new DeliveryStation();
  } else if (type == "ss") {
    m = new StorageStation();
  } else {
    cerr << "Cannot handle type " << type << endl;
    cerr << "Has to be either bs, rs, cs, ds or ss" << endl;
    return 1;
    //throw invalid_argument("Cannot handle this type");
  }

  cout << "Created Machine of type " << type << endl;

  m->connectPLC(argv[2], port_plc);

  cout << "Connected to PLC (via modbus)" << endl;

  MachineProtoServer::initialize(port_pb, m);
  cout << "Server initialized" <<endl;
  cout << "If there is a disconnect, process will exit." <<endl;
  MachineProtoServer::getServer().signal_disconnected().connect(
      [] (int clientId, boost::system::error_code endpoint) -> void {
        cout << "Client disconnected..." << endl;
        cout << "Bye. Going to exit!" << endl;
        exit(0);
        });
  cout << "Going to loop now infinitely..." << endl;
  MachineProtoServer::getServer().processQueue();
  // will be never reached
  return 0;
}

  

  


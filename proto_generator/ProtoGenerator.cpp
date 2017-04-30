#include "../protobuf/MachineInstructions.pb.h"
#include <protobuf_comm/client.h>
#include <vector>
#include <string>
using namespace google::protobuf;
using namespace protobuf_comm;
using namespace llsf_msgs;

void printProtoMsg(uint16_t component_id,
    uint16_t msg_type, std::shared_ptr<Message> msg) {
  //msg->PrintDebugString();
}

int main(int argc, char** argv) {
  if (argc != 3) {
    cerr << "Usage: " << argv[0] << " <ip address> <port>" << endl;
    return 1;
  }
  unsigned short port = (unsigned short) atoi(argv[2]);

  vector<string> path;
  path.push_back("../protobuf");
  protobuf_comm::ProtobufStreamClient c(path);
  c.signal_received().connect(printProtoMsg);
  c.async_connect(argv[1], port);
  int answer;
  shared_ptr<Message> m;
  do {
    cout << "Send a Protobuf package:" <<endl;
    cout << "  [0] exit" <<endl
      << "  [1] Set light to green" <<endl
      << "  [2] Move conveyor belt to start" <<endl
      << "  [3] Move conveqor belt to end" <<endl;
    cin >> answer;
    switch (answer) {
      case 0: break;
      case 1: {
                cout << "Get green light msg" << endl;
              auto mx = new SetSignalLight();
              mx->set_red(OFF);
              mx->set_green(ON);
              mx->set_yellow(OFF);
              cout << "Do a reset" << endl;
              m.reset(mx);
              cout << "Ok" << endl;
              break;}
      case 2: {
              auto mx = new MoveConveyorBelt();
              mx->set_direction(BACKWARD);
              mx->set_stop_sensor(SENSOR_INPUT);
              m.reset(mx);
              break;
              }
      case 3: {
              auto mx = new MoveConveyorBelt();
              mx->set_direction(FORWARD);
              mx->set_stop_sensor(SENSOR_OUTPUT);
              m.reset(mx);
              break;
              }
      default:
              cout << "Did not understand your answer " << answer <<". Please try again." << endl;
              continue;
    }
    if (answer) {
      cout << "Now sending message" << endl;
      c.send(m);
      cout << "Finished sending" << endl;
    }
  } while (answer);
  cout << "Bye." << endl;
}




  


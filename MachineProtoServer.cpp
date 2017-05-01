#include "MachineProtoServer.h"
#include "Machine.h"
#include <vector>
#include <string>
#include "timeoutException.h"
#include <iostream>
#include "protobuf/MachineInstructions.pb.h"

using namespace std;
using namespace protobuf_comm;
using namespace llsf_msgs;

MachineProtoServer::MachineProtoServer( unsigned short port, Machine* machine):
    ProtobufStreamServer(port, proto_path_), machine_(machine), connected_(false) {
  // TODO: check, if I also have to set some ids in case of signal_connected().
  //       I guess I wont need signal_disconnected and signal_failed.
  signal_connected().connect([this] (auto clientId, auto endpoint) -> void {
      // NOTE: No idea, if I'm goining to need this variable anywhere...
      // Help! What to do with this signal?
      this->connected_ = true;
      });
  signal_received().connect(boost::bind(&MachineProtoServer::handleProtobufMsg, this, _1, _2, _3, _4));
}


void MachineProtoServer::handleProtobufMsg(ProtobufStreamServer::ClientID client,
    uint16_t component_id, uint16_t msg_type, std::shared_ptr<google::protobuf::Message> msg) {
  auto m = dynamic_pointer_cast<InstructMachine> ( msg);
  if (m) {
    // TODO: what to do with member machine and member set?
    try {
      if (m->has_light_state()) {
        machine_->handleProtobufMsg(*(m->mutable_light_state()), *this);
      }
      if (m->has_conveyor_belt()) {
        machine_->handleProtobufMsg(*(m->mutable_conveyor_belt()), *this);
      }
      if (m->has_bs()) {
        machine_->handleProtobufMsg(*(m->mutable_bs()), *this);
      }
      if (m->has_ss()) {
        machine_->handleProtobufMsg(*(m->mutable_ss()), *this);
      }
      if (m->has_rs()) {
        machine_->handleProtobufMsg(*(m->mutable_rs()), *this);
      }
      if (m->has_cs()) {
        machine_->handleProtobufMsg(*(m->mutable_cs()), *this);
      }
      if (m->has_ds()) {
        machine_->handleProtobufMsg(*(m->mutable_ds()), *this);
      }
      auto repl = make_shared<MachineReply>();
      repl->set_id(m->id());
      repl->set_machine(m->machine());
      repl->set_set(MACHINE_REPLY_FINISHED);
    } catch (const exception& e) {
      cout << e.what() << endl;
    }
  } else {
    cerr << "Message " << msg->GetTypeName() << " cannot be parsed!" << endl;
  }


  //try {
  //  machine_->handleProtobufMsg(*msg, *this);
  //} catch (timeoutException& e) {
  //  cout << "Operation timed out! Goin to reset the machine. " <<
  //    "Going to ignore it on protobuf side (not good)." << endl;
  //  machine_->reset(); // TODO This will create infiniete loops. Change resets behaviour or so...
  //} catch (runtime_error& e) {
  //  cout << "Got runtime error: " << e.what() << endl;
  //} catch (invalid_argument& e) {
  //  cout << "Got invalid argument error: " << e.what() << endl;
  //}
}
  


MachineProtoServer& MachineProtoServer::initialize( unsigned short port, Machine* machine) {
  assert( not instance_);
  instance_ = new MachineProtoServer(port, machine);
  return *instance_;
}



// TODO: adapt this path to the actual path of the directory with all the .proto files in it.
vector<string> MachineProtoServer::proto_path_ = {"./protobuf"};
MachineProtoServer* MachineProtoServer::instance_ = nullptr;

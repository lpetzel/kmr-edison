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
    // Lets push m to queue
    {
      unique_lock<mutex> l(lock_);
      // Abort current job?
      bool abort = false;
      if( (not msg_.empty()) and messageOverwrites(m, msg_.front()))
        abort = true;
      for(auto iter = msg_.begin(); iter != msg_.end();) {
        if( messageOverwrites(m, *iter) ) {
          msg_.erase(iter++);
          cout << "Kicked out message" << endl;
        } else {
          ++iter;
        }
      }
      msg_.push_back(m);
      // Abort _before_ releasing the lock
      if (abort) {
        lock_guard<mutex> l2(machine_->lock_);
        machine_->abort_operation_ = true;
        cout << "Set abort flag" << endl;
      }
    }
    // After releasing all locks:
    // signal new message
    msg_available_.notify_all();
  } else {
    cerr << "Message " << msg->GetTypeName() << " cannot be parsed!" << endl;
  }
}
  

void MachineProtoServer::processQueue() {
  shared_ptr<InstructMachine> m;
  for (bool first = true; ;first = false) {
    // Remove handled message and get a new message from queue
    {
      unique_lock<mutex> l(lock_);
      if (not first)
        msg_.pop_front();
      while (msg_.empty()) {
        // wait for message to arrive
        msg_available_.wait(l);
      }
      m = msg_.front();
    }
    // Delete possible abort flag
    {
      lock_guard<mutex> l2(machine_->lock_);
      machine_->abort_operation_ = false;
    }
    // Tell machine to handle the sub messages:
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
      bool aborted;
      {
        lock_guard<mutex> l2(machine_->lock_);
        aborted = machine_->abort_operation_;
      }
      if (not aborted)
        send_to_all(repl);
    } catch (const exception& e) {
      cout << e.what() << endl;
    }
  }
}



bool MachineProtoServer::messageOverwrites( shared_ptr<InstructMachine> n,
    shared_ptr<InstructMachine> old) {
  return n->set() == old->set();
}


MachineProtoServer& MachineProtoServer::initialize( unsigned short port, Machine* machine) {
  assert( not instance_);
  instance_ = new MachineProtoServer(port, machine);
  return *instance_;
}



// TODO: adapt this path to the actual path of the directory with all the .proto files in it.
vector<string> MachineProtoServer::proto_path_ = {"./protobuf"};
MachineProtoServer* MachineProtoServer::instance_ = nullptr;

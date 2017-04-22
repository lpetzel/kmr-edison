#pragma once

#include <protobuf_comm/server.h>
#include <vector>
#include <string>

class Machine;
class MachineProtoServer;

// Sigleton class
// Connection to refbox
// Currently, it is supposed to connect to only one refbox.
// If multiple clients connect, each message is sent to all of them!
class MachineProtoServer : public protobuf_comm::ProtobufStreamServer {
  private:
    static MachineProtoServer* instance_;
    Machine* machine_;
    bool connected_;

  public:
    static std::vector<std::string> proto_path_;
    MachineProtoServer( unsigned short port, Machine* machine);

    Machine& getMachine() {return *machine_;}
    const Machine& getMachine() const {return *machine_;}

    void handleProtobufMsg(protobuf_comm::ProtobufStreamServer::ClientID client,
        uint16_t component_id, uint16_t msg_type, std::shared_ptr<google::protobuf::Message> msg);

    static MachineProtoServer& getServer() { return *instance_;}
    static bool initialized() { return instance_;}
    static MachineProtoServer& initialize( unsigned short port, Machine* machine);
};



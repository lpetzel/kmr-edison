// The MachineProtoServer is a ProtobufStreamServer.
// It handles the connection to the refbox.
// Currently it is a server, which will connect to exaclty one client.
// When the client disconnects, the process will be killed.
#pragma once

#include <protobuf_comm/server.h>
#include <vector>
#include <string>
#include <list>
#include <mutex>
#include <memory>
#include <condition_variable>
#include "protobuf/MachineInstructions.pb.h"

class Machine;
class MachineProtoServer;

// Sigleton class
// Connection to refbox
// Currently, it is supposed to connect to only one refbox.
// If multiple clients connect, each message is sent to all of them!
class MachineProtoServer : public protobuf_comm::ProtobufStreamServer {
  private:
    // signleton
    static MachineProtoServer* instance_;

    // Normal members:
    // A machine object, which has to be created before.
    // It won't be deleted by destructor.
    Machine* machine_;
    // if this is true, a refbox has connected.
    // Currently unused.
    bool connected_;

    // This lock is used for the msg_available_ condition and
    // the msg_ message queue.
    std::mutex lock_;
    // This condition_variable will be triggered, if a new message arrives.
    // The processQueue() can sleep, if nothing has to be done.
    std::condition_variable msg_available_;
    // Message queue, now implemented as a list, so that iterating
    // and removing are easy.
    std::list<std::shared_ptr<llsf_msgs::InstructMachine>> msg_;
  public:

    // Path, where the .proto messages lie (for server initialisation)
    static std::vector<std::string> proto_path_;
    // Constructor
    MachineProtoServer( unsigned short port, Machine* machine);

    // getter
    Machine& getMachine() {return *machine_;}
    const Machine& getMachine() const {return *machine_;}

    // Callback for arriving protobuf messages:
    // client, component_id and msg_type are unused.
    // Currently only InstructMachine messages get parsed.
    // They are added to the queue msg_.
    void handleProtobufMsg(protobuf_comm::ProtobufStreamServer::ClientID client,
        uint16_t component_id, uint16_t msg_type, std::shared_ptr<google::protobuf::Message> msg);

    // Endless loop, which processes messages in the msg_ queue
    void processQueue();
    
    // Some messages overwrite other queued messages.
    // This method is a predicate for this. n is the new message.
    // old is the old message. true means, that the old message is removed.
    // If the old message is currently executed, the command will be aborted.
    // Currently Messages with the same "set" attribute will overwrite each other.
    static bool messageOverwrites( std::shared_ptr<llsf_msgs::InstructMachine> n,
        std::shared_ptr<llsf_msgs::InstructMachine> old);

    // static singleton methods
    static MachineProtoServer& getServer() { return *instance_;}
    static bool initialized() { return instance_;}
    static MachineProtoServer& initialize( unsigned short port, Machine* machine);

};



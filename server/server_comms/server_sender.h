#ifndef SERVER_SENDER_H
#define SERVER_SENDER_H

#include <atomic>

#include "server_protocol.h"

#include "common/comms/sender.h"
#include "common/hands_on_threads/queue.h"


class ServerSender: public Sender
{
private:
    ServerProtocol& protocol;
    Queue<Gamestate>& server_messages;
    void send_data() override;
public:
    ServerSender(std::atomic_bool&, ServerProtocol&, Queue<Gamestate>&);
    ~ServerSender() override = default;
};


#endif
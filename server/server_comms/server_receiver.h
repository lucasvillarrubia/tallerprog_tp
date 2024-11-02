#ifndef SERVER_RECEIVER_H
#define SERVER_RECEIVER_H


#include <atomic>

#include "server_protocol.h"

#include "common/gamedata.h"
#include "common/comms/receiver.h"
#include "common/hands_on_threads/queue.h"


class ServerReceiver: public Receiver
{
private:
    ServerProtocol& protocol;
    Queue<Gameaction>& clients_actions;
    void receive_data() override;
public:
    ServerReceiver(std::atomic_bool&, ServerProtocol&, Queue<Gameaction>&);
    ~ServerReceiver() override = default;
};


#endif

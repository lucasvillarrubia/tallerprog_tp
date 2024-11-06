#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H


#include "client_protocol.h"

#include "common/gamedata.h"
#include "common/comms/receiver.h"
#include "common/hands_on_threads/queue.h"


class ClientReceiver: public Receiver
{
private:
    ClientProtocol& protocol;
    Queue<Gamestate>& server_updates;
    void receive_data() override;
public:
    explicit ClientReceiver(std::atomic_bool&, ClientProtocol&, Queue<Gamestate>&);
    ~ClientReceiver() override = default;
};


#endif

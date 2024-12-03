#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H


#include "client_protocol.h"
#include "client/state_manager.h"

#include "common/gamedata.h"
#include "common/comms/receiver.h"
#include "common/hands_on_threads/queue.h"


class ClientReceiver: public Receiver
{
private:
    ClientProtocol& protocol;
    Queue<Gamestate>& server_updates;
    StateManager& state;
    void receive_data() override;
public:
    // explicit ClientReceiver(std::atomic_bool&, ClientProtocol&, Queue<Gamestate>&);
    explicit ClientReceiver(std::atomic_bool&, ClientProtocol&, Queue<Gamestate>&, StateManager&);
    ~ClientReceiver() override = default;
};


#endif

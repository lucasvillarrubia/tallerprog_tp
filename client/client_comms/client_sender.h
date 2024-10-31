#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H


#include "client_protocol.h"

#include "common/gameaction.h"
#include "common/comms/sender.h"
#include "common/hands_on_threads/queue.h"


class ClientSender: public Sender
{
private:
    ClientProtocol& protocol;
    Queue<Gameaction>& player_actions;
    void send_data() override;
public:
    ClientSender(std::atomic_bool&, ClientProtocol&, Queue<Gameaction>&);
    ~ClientSender() override = default;
};


#endif

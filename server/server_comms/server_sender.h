#ifndef SERVER_SENDER_H
#define SERVER_SENDER_H

#include <atomic>

#include "server_protocol.h"

#include "common/comms/sender.h"


class ServerSender: public Sender
{
private:
    ServerProtocol& protocol;
    void send_data() override;
public:
    ServerSender(ServerProtocol&, std::atomic_bool&);
    ~ServerSender() override = default;
};


#endif
#ifndef SERVER_RECEIVER_H
#define SERVER_RECEIVER_H


#include <atomic>

#include "server_protocol.h"

#include "common/comms/receiver.h"


class ServerReceiver: public Receiver
{
private:
    ServerProtocol& protocol;
    void receive_data() override;
public:
    ServerReceiver(ServerProtocol&, std::atomic_bool&);
    ~ServerReceiver() override = default;
};


#endif

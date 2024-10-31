#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H


#include "common/hands_on_sockets/socket.h"
#include "common/comms/protocol.h"


class ClientProtocol: public Protocol
{
public:
    ClientProtocol(Socket&&, std::atomic_bool&);
    void send_message() override;
    void receive_message() override;
    ~ClientProtocol() override = default;
};


#endif
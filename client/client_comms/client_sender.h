#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H


#include "client_protocol.h"
#include "common/comms/sender.h"


class ClientSender: public Sender
{
private:
    ClientProtocol& protocol;
    void send_data() override;
public:
    explicit ClientSender(ClientProtocol&, std::atomic_bool&);
    ~ClientSender() override = default;
};


#endif

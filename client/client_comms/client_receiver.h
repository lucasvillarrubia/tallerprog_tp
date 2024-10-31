#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H


#include "client_protocol.h"

#include "common/comms/receiver.h"


class ClientReceiver: public Receiver
{
private:
    ClientProtocol& protocol;
    void receive_data() override;
public:
    explicit ClientReceiver(ClientProtocol&, std::atomic_bool&);
    ~ClientReceiver() override = default;
};


#endif

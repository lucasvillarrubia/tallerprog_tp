//
// Created by lucas on 27/10/24.
//

#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H


#include "common/thread.h"
#include "client_protocol.h"


class ClientReceiver: public Thread
{
private:
    ClientProtocol& protocol;
public:
    explicit ClientReceiver(ClientProtocol&);
    void run() override;
    ~ClientReceiver();
};


#endif

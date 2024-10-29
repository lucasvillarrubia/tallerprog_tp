//
// Created by lucas on 27/10/24.
//

#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H


#include "common/thread.h"
#include "client_protocol.h"


class ClientSender: public Thread
{
private:
    ClientProtocol& protocol;
public:
    explicit ClientSender(ClientProtocol&);
    void run() override;
    ~ClientSender();
};


#endif

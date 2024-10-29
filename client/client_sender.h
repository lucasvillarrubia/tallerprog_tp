//
// Created by lucas on 27/10/24.
//

#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H
#include "common/thread.h"
#include "server/server_protocol.h"


class ClientSender: public Thread
{
private:
    ServerProtocol& protocol;
};


#endif

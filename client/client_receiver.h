//
// Created by lucas on 27/10/24.
//

#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H
#include "common/thread.h"
#include "server/server_protocol.h"


class ClientReceiver: public Thread
{
private:
    ServerProtocol& protocol;
};


#endif

#ifndef LOCAL_PLAYER_H
#define LOCAL_PLAYER_H


#include "client_protocol.h"
#include "client_receiver.h"
#include "client_sender.h"


class LocalPlayer {
private:
    ClientProtocol protocol;
    ClientSender sender;
    ClientReceiver receiver;
};


#endif

#ifndef LOCAL_PLAYER_H
#define LOCAL_PLAYER_H


#include "client_protocol.h"
#include "client_receiver.h"
#include "client_sender.h"
#include "common/gameaction.h"
#include "common/gamestate.h"
#include "common/queue.h"


class LocalPlayer {
private:
    ClientProtocol protocol;
    ClientSender sender;
    ClientReceiver receiver;
    Queue<Gameaction>& player_actions;
    Queue<Gamestate>& server_updates;
public:
    LocalPlayer(Socket&&, Queue<Gameaction>&, Queue<Gamestate>&);
    ~LocalPlayer();
};


#endif

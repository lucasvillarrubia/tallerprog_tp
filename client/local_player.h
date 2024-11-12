#ifndef LOCAL_PLAYER_H
#define LOCAL_PLAYER_H


#include "client_comms/client_protocol.h"
#include "client_comms/client_receiver.h"
#include "client_comms/client_sender.h"
#include "common/gamedata.h"
#include "common/hands_on_sockets/socket.h"
#include "common/hands_on_threads/queue.h"


class LocalPlayer
{
private:
    std::atomic_bool& is_online;
    ClientProtocol protocol;
    ClientSender sender;
    ClientReceiver receiver;
public:
    LocalPlayer(Socket&&, Queue<Gameaction>&, Queue<Gamestate>&, std::atomic_bool&);
    void start_communication();
    void end_connection();
    LocalPlayer(const LocalPlayer&) = delete;
    LocalPlayer& operator=(const LocalPlayer&) = delete;
    LocalPlayer(LocalPlayer&& other) = delete;
    LocalPlayer& operator=(LocalPlayer&& other) = delete;
    ~LocalPlayer() = default;
};


#endif

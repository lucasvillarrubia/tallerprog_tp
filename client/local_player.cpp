#include "local_player.h"


LocalPlayer::LocalPlayer(Socket&& skt, Queue<Gameaction>& evn, Queue<Gamestate>& upd):
        is_online(false),
        protocol(std::move(skt), is_online),
        sender(is_online, protocol, evn),
        receiver(is_online, protocol, upd) {}

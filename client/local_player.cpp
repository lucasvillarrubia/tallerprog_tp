#include "local_player.h"


LocalPlayer::LocalPlayer(Socket&& skt, Queue<Gameaction>& evn, Queue<Gamestate>& upd, std::atomic_bool& con_stat):
        is_online(con_stat),
        protocol(std::move(skt), is_online),
        sender(is_online, protocol, evn),
        receiver(is_online, protocol, upd)
{
    is_online.store(true);
}

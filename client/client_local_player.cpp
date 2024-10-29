#include "local_player.h"


LocalPlayer::LocalPlayer(Socket&& skt, Queue<Gameaction>& evn, Queue<Gamestate>& upd): protocol(std::move(skt)), sender(protocol), receiver(protocol), player_actions(evn), server_updates(upd) {}

LocalPlayer::~LocalPlayer()
{

}


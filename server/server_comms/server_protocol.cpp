#include "server_protocol.h"

#include <utility>


ServerProtocol::ServerProtocol(Socket&& skt, std::atomic_bool& connection_status): Protocol(std::move(skt), connection_status) {}

void ServerProtocol::send_message(const Gamestate& message)
{
    if (client_disconnected.load()) return;
    send_single_int_message(message.player_id);
    send_single_int_message(message.type);
    send_single_int_message(message.key);
}

void ServerProtocol::receive_message(Gameaction& received)
{
    if (client_disconnected.load()) return;
    uint8_t player, type, key;
    receive_single_int_message(player);
    receive_single_int_message(type);
    receive_single_int_message(key);
    received = Gameaction(player, type, key);
}

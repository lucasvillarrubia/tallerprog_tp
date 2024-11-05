#include "server_protocol.h"

#include <utility>


ServerProtocol::ServerProtocol(Socket&& skt, std::atomic_bool& connection_status): Protocol(std::move(skt), connection_status) {}

void ServerProtocol::send_message(const Gamestate& message)
{
    if (not client_is_connected.load()) return;
    send_single_8bit_int(message.player_id);
    send_single_float(message.pos_X);
    send_single_float(message.pos_Y);
    send_single_8bit_int(message.is_running);
    send_single_8bit_int(message.is_jumping);
    send_single_8bit_int(message.is_flapping);
    send_single_8bit_int(message.move_direction);
    send_single_float(message.jump_speed);
}

void ServerProtocol::receive_message(Gameaction& received)
{
    if (not client_is_connected.load()) return;
    uint8_t player, type, key;
    receive_single_8bit_int(player);
    receive_single_8bit_int(type);
    receive_single_8bit_int(key);
    received = Gameaction(player, type, key);
}

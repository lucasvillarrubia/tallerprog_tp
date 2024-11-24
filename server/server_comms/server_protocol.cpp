#include "server_protocol.h"

#include <utility>


ServerProtocol::ServerProtocol(Socket&& skt, std::atomic_bool& connection_status): Protocol(std::move(skt), connection_status) {}

void ServerProtocol::send_init_duck_message(const Gamestate& message)
{
    if (not client_is_connected.load()) return;
    send_single_8bit_int(message.type);
    send_single_8bit_int(message.player_id);
    send_single_float(message.pos_X);
    send_single_float(message.pos_Y);
    send_single_8bit_int(message.is_running);
    send_single_8bit_int(message.is_jumping);
    send_single_8bit_int(message.is_flapping);
    send_single_8bit_int(message.move_direction);
    send_single_8bit_int(message.is_alive);
    send_single_float(message.jump_speed);
}

void ServerProtocol::send_single_duck_position_message(const int id, const Coordinates& position)
{
    send_single_8bit_int(id);
    send_single_float(position.pos_X);
    send_single_float(position.pos_Y);
}

void ServerProtocol::send_ducks_positions_message(const Gamestate& message)
{
    if (not client_is_connected.load()) return;
    send_single_8bit_int(message.type);
    uint8_t positions_count = message.positions_by_id.size();
    send_single_8bit_int(positions_count);
    for (auto& [id, position] : message.positions_by_id)
    {
        send_single_duck_position_message(id, position);
    }
}

void ServerProtocol::send_duck_state_message(const Gamestate& message)
{
    if (not client_is_connected.load()) return;
    send_single_8bit_int(message.type);
    send_single_8bit_int(message.player_id);
    send_single_8bit_int(message.is_running);
    send_single_8bit_int(message.is_jumping);
    send_single_8bit_int(message.is_flapping);
    send_single_8bit_int(message.move_direction);
    send_single_8bit_int(message.is_alive);
}

void ServerProtocol::receive_message(Gameaction& received)
{
    if (not client_is_connected.load()) return;
    uint8_t player, match, type, key, is_multiplayer;
    receive_single_8bit_int(player);
    receive_single_8bit_int(match);
    receive_single_8bit_int(type);
    receive_single_8bit_int(key);
    receive_single_8bit_int(is_multiplayer);
    received = Gameaction(player, match, type, key, is_multiplayer);
}

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
    send_single_8bit_int(message.is_grabbing);
	send_single_8bit_int(message.is_shooting);
	send_single_8bit_int(message.with_gun);
	send_single_8bit_int(message.with_armor);
	send_single_8bit_int(message.with_helmet);
    send_single_8bit_int(message.move_direction);
    send_single_float(message.jump_speed);
}

void ServerProtocol::send_init_gun_message(const Gamestate& message) {
	send_single_8bit_int(message.type);
	send_single_8bit_int(message.gun_id);
	send_single_8bit_int(message.type_gun);
    send_single_float(message.pos_X);
    send_single_float(message.pos_Y);
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

void ServerProtocol::send_guns_positions_message(const Gamestate& message)
{
    send_single_8bit_int(message.type);
    uint8_t positions_count = message.guns_positions_by_type.size();
    send_single_8bit_int(positions_count);
    for (auto& [id, gunData] : message.guns_positions_by_type)
    {
        send_single_8bit_int(id);
        send_single_8bit_int(gunData.first);
        send_single_float(gunData.second.pos_X);
    	send_single_float(gunData.second.pos_Y);
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
    send_single_8bit_int(message.is_grabbing);
	send_single_8bit_int(message.is_shooting);
	send_single_8bit_int(message.with_gun);
	send_single_8bit_int(message.with_armor);
	send_single_8bit_int(message.with_helmet);
    send_single_8bit_int(message.move_direction);
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

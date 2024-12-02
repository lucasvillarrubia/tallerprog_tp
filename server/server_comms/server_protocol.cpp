#include "server_protocol.h"

#include <utility>
#include <iostream>

ServerProtocol::ServerProtocol(Socket&& skt, std::atomic_bool& connection_status): Protocol(std::move(skt), connection_status) {}

void ServerProtocol::send_init_duck_message(const Gamestate& message)
{
    // std::cout << "Sending init duck message" << std::endl;
    if (not client_is_connected.load()) return;
    // std::cout << "Client is connected" << std::endl;
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
    send_single_8bit_int(message.color.r);
    send_single_8bit_int(message.color.g);
    send_single_8bit_int(message.color.b);
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
    uint8_t speeds_count = message.speeds_by_id.size();
    send_single_8bit_int(speeds_count);
    for (auto& [id, speed] : message.speeds_by_id)
    {
        send_single_8bit_int(id);
        send_single_float(speed);
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
    send_single_8bit_int(message.is_slipping);
    send_single_8bit_int(message.is_pointing_upwards);
    send_single_8bit_int(message.is_ducking);
}


void ServerProtocol::send_init_gun_message(const Gamestate& message) {
	if (not client_is_connected.load()) return;
	send_single_8bit_int(message.type);
    send_single_8bit_int(message.object_id);
    send_single_float(message.pos_X);
    send_single_float(message.pos_Y);
    send_single_8bit_int(message.type_gun);
    send_single_8bit_int(message.move_direction);
}
void ServerProtocol::send_guns_positions_message(const Gamestate& message) {
	if (not client_is_connected.load()) return;
    send_single_8bit_int(message.type);
    uint8_t positions_count = message.guns_positions_by_id.size();
    send_single_8bit_int(positions_count);
    for (auto& [id, gun_data] : message.guns_positions_by_id)
    {
    	send_single_8bit_int(id);
        send_single_8bit_int(gun_data.first.type);
        send_single_8bit_int(gun_data.first.right);
        send_single_8bit_int(gun_data.first.shooting);
    	send_single_float(gun_data.second.pos_X);
    	send_single_float(gun_data.second.pos_Y);
    }
}
void ServerProtocol::send_bullet_init_message(const Gamestate& message) {
	if (not client_is_connected.load()) return;
	send_single_8bit_int(message.type);
    send_single_8bit_int(message.object_id);
    send_single_float(message.pos_X);
    send_single_float(message.pos_Y);
    send_single_8bit_int(message.type_gun);
    send_single_8bit_int(message.move_direction);
}
void ServerProtocol::send_bullets_positions_message(const Gamestate& message) {
	if (not client_is_connected.load()) return;
	send_single_8bit_int(message.type);
	send_single_8bit_int(message.bullet_flag);
	uint8_t positions_count = message.bullets_positions_by_id.size();
    send_single_8bit_int(positions_count);
	for (auto& [id, bullet_data] : message.bullets_positions_by_id){
    	send_single_8bit_int(id);
    	send_single_float(bullet_data.pos_X);
    	send_single_float(bullet_data.pos_Y);
    }
}
void ServerProtocol::send_bullet_destroy_message(const Gamestate& message) {
	if (not client_is_connected.load()) return;
	send_single_8bit_int(message.type);
    send_single_8bit_int(message.object_id);
}

void ServerProtocol::send_match_error_message(const Gamestate& message)
{
    if (not client_is_connected.load()) return;
    send_single_8bit_int(message.type);
    send_single_8bit_int(message.player_id);
    send_single_8bit_int(message.match_errors_flag);
    std::vector<char> message_bytes(message.error_msg.begin(), message.error_msg.end());
    send_string(message_bytes);
}

void ServerProtocol::send_match_info_message(const Gamestate& message)
{
    if (not client_is_connected.load()) return;
    send_single_8bit_int(message.type);
    send_single_8bit_int(message.player_id);
    send_single_8bit_int(message.match_errors_flag);
    send_single_8bit_int(message.match_id);
    send_single_8bit_int(message.player_count);
}

void ServerProtocol::send_matches_info_message(const Gamestate& message)
{
    if (not client_is_connected.load()) return;
    send_single_8bit_int(message.type);
    send_single_8bit_int(message.player_id);
    uint8_t matches_count = message.matches_info.size();
    send_single_8bit_int(matches_count);
    for (auto& match_info : message.matches_info)
    {
        send_single_8bit_int(match_info.player_id);
        send_single_8bit_int(match_info.match_id);
        send_single_8bit_int(match_info.players_count);
    }
}


void ServerProtocol::send_explosion_message(const Gamestate& message) {
	if (not client_is_connected.load()) return;
	send_single_8bit_int(message.type);
	send_single_8bit_int(message.bullet_flag);
	send_single_8bit_int(message.object_id);
	send_single_float(message.match_errors_flag);
	
}

void ServerProtocol::send_round_ended_message(const Gamestate& message)
{
    if (not client_is_connected.load()) return;
    send_single_8bit_int(message.type);
    send_single_8bit_int(message.player_id);
    send_single_8bit_int(message.round);
}

void ServerProtocol::send_exit_message(const Gamestate& message)
{
    if (not client_is_connected.load()) return;
    send_single_8bit_int(message.type);
    send_single_8bit_int(message.player_id);
    std::vector<char> message_bytes(message.error_msg.begin(), message.error_msg.end());
    send_string(message_bytes);
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

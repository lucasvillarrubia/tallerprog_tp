#include "client_protocol.h"

#include <iostream>
#include <utility>


ClientProtocol::ClientProtocol(Socket&& skt, std::atomic_bool& connection_status): Protocol(std::move(skt), connection_status) {}

void ClientProtocol::send_message(const Gameaction& message)
{
    // std::cout << "entré al protocolo del cliente\n";
    if (not client_is_connected.load()) return;
    send_single_8bit_int(message.player_id);
    send_single_8bit_int(message.match);
    send_single_8bit_int(message.type);
    send_single_8bit_int(message.key);
    send_single_8bit_int(message.is_multiplayer);
}

void ClientProtocol::receive_init_character_message(Gamestate& received)
{
    if (not client_is_connected.load()) return;
    uint8_t character_id, run_state, jump_state, flap_state, move_direction, life;
    float pos_X, pos_Y, velocity_Y;
    receive_single_8bit_int(character_id);
    receive_single_float(pos_X);
    receive_single_float(pos_Y);
    receive_single_8bit_int(run_state);
    receive_single_8bit_int(jump_state);
    receive_single_8bit_int(flap_state);
    receive_single_8bit_int(move_direction);
    receive_single_8bit_int(life);
    receive_single_float(velocity_Y);
    received = Gamestate(character_id, pos_X, pos_Y, run_state, jump_state, flap_state, move_direction, life, velocity_Y);
}

// void ClientProtocol::receive_single_character_position_message()
// {
// }

void ClientProtocol::receive_characters_positions_message(Gamestate& received)
{
    if (not client_is_connected.load()) return;
    std::map<int, Coordinates> positions_by_id;
    uint8_t positions_count;
    receive_single_8bit_int(positions_count);
    for (int i = 0; i < positions_count; i++)
    {
        uint8_t id;
        float pos_X, pos_Y;
        receive_single_8bit_int(id);
        receive_single_float(pos_X);
        receive_single_float(pos_Y);
        positions_by_id.insert({id, {pos_X, pos_Y}});
    }
    received = Gamestate(positions_by_id);
}

void ClientProtocol::receive_character_update_message(Gamestate& received)
{
    if (not client_is_connected.load()) return;
    uint8_t player_id, run, jump, flap, direction, life;
    receive_single_8bit_int(player_id);
    receive_single_8bit_int(run);
    receive_single_8bit_int(jump);
    receive_single_8bit_int(flap);
    receive_single_8bit_int(direction);
    receive_single_8bit_int(life);
    received = Gamestate(player_id, run, jump, flap, direction, life);
}


void ClientProtocol::receive_init_gun_message(Gamestate& received) {
	if (not client_is_connected.load()) return;
    uint8_t gun_id, type_gun, direction;
    float pos_X, pos_Y;
    receive_single_8bit_int(gun_id);
    receive_single_float(pos_X);
    receive_single_float(pos_Y);
    receive_single_8bit_int(type_gun);
    receive_single_8bit_int(direction);
    received = Gamestate(gun_id, pos_X, pos_Y, type_gun, direction);
}

void ClientProtocol::receive_guns_positions_message(Gamestate& received) {
	if (not client_is_connected.load()) return;
	uint8_t positions_count;
	std::map<int, std::pair<DrawingData, Coordinates>> guns_positions;
	receive_single_8bit_int(positions_count);
    for (int i = 0; i < positions_count; i++) {
    	uint8_t id, type_gun, direction;
        float pos_X, pos_Y;
        receive_single_8bit_int(id);
        receive_single_8bit_int(type_gun);
        receive_single_8bit_int(direction);
        receive_single_float(pos_X);
        receive_single_float(pos_Y);
        DrawingData gun_data = { type_gun, direction };
        Coordinates gun_position = { pos_X, pos_Y};
        guns_positions.insert({id, std::make_pair(gun_data, gun_position)});
    }
    received = Gamestate(guns_positions);    
}

void ClientProtocol::receive_bullet_init_message(Gamestate& received) {
	if (not client_is_connected.load()) return;
    uint8_t gun_id, type_gun, direction;
    float pos_X, pos_Y;
    receive_single_8bit_int(gun_id);
    receive_single_float(pos_X);
    receive_single_float(pos_Y);
    receive_single_8bit_int(type_gun);
    receive_single_8bit_int(direction);
    received = Gamestate(gun_id, type_gun, direction, pos_X, pos_Y);
}

void ClientProtocol::receive_bullets_positions_message(Gamestate& received) {
	if (not client_is_connected.load()) return;
    std::map<int, Coordinates> bullets_positions_by_id;
    uint8_t flag, positions_count;
    receive_single_8bit_int(flag);
    receive_single_8bit_int(positions_count);
    for (int i = 0; i < positions_count; i++)
    {
        uint8_t id;
        float pos_X, pos_Y;
        receive_single_8bit_int(id);
        receive_single_float(pos_X);
        receive_single_float(pos_Y);
        bullets_positions_by_id.insert({id, {pos_X, pos_Y}});
    }
    received = Gamestate(flag, bullets_positions_by_id);
}
void ClientProtocol::receive_bullet_destroy_message(Gamestate& received) {
    if (not client_is_connected.load()) return;
    uint8_t bullet_id;
    receive_single_8bit_int(bullet_id);
    received = Gamestate(bullet_id);
}

void ClientProtocol::receive_message(Gamestate& received)
{
    if (not client_is_connected.load()) return;
    uint8_t message_type;
    receive_single_8bit_int(message_type);
    switch (message_type)
    {
    case 1:
        receive_init_character_message(received);
        break;
    case 2:
        receive_characters_positions_message(received);
        break;
	case 5:
        receive_init_gun_message(received);
        break;
	case 6:
        receive_guns_positions_message(received);
        break;
	case 7:
        receive_bullet_init_message(received);
        break;
	case 8:
        receive_bullets_positions_message(received);
        break;
	case 9:
        receive_bullet_destroy_message(received);
        break;
    default:
        receive_character_update_message(received);
        break;
    }
}

// bool ClientProtocol::theres_more_data_per_code(int msg_code) {
//     return (msg_code == PICKUP_MESSAGE_CODE) ? true : false;
// }

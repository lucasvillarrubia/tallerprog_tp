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
}

void ClientProtocol::receive_init_character_message(Gamestate& received)
{
    if (not client_is_connected.load()) return;
    uint8_t character_id, run_state, jump_state, flap_state, move_direction;
    float pos_X, pos_Y, velocity_Y;
    receive_single_8bit_int(character_id);
    receive_single_float(pos_X);
    receive_single_float(pos_Y);
    receive_single_8bit_int(run_state);
    receive_single_8bit_int(jump_state);
    receive_single_8bit_int(flap_state);
    receive_single_8bit_int(move_direction);
    receive_single_float(velocity_Y);
    received = Gamestate(character_id, pos_X, pos_Y, run_state, jump_state, flap_state, move_direction, velocity_Y);
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
    uint8_t player_id, run, jump, flap, direction;
    receive_single_8bit_int(player_id);
    receive_single_8bit_int(run);
    receive_single_8bit_int(jump);
    receive_single_8bit_int(flap);
    receive_single_8bit_int(direction);
    received = Gamestate(player_id, run, jump, flap, direction);
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
    default:
        receive_character_update_message(received);
        break;
    }
}

// bool ClientProtocol::theres_more_data_per_code(int msg_code) {
//     return (msg_code == PICKUP_MESSAGE_CODE) ? true : false;
// }

#include "client_protocol.h"

#include <iostream>
#include <utility>


ClientProtocol::ClientProtocol(Socket&& skt, std::atomic_bool& connection_status): Protocol(std::move(skt), connection_status) {}

void ClientProtocol::send_message(const Gameaction& message)
{
    // std::cout << "entré al protocolo del cliente\n";
    if (not client_is_connected.load()) return;
    send_single_int(message.player_id);
    send_single_int(message.type);
    send_single_int(message.key);
}

void ClientProtocol::receive_message(Gamestate& received)
{
    if (not client_is_connected.load()) return;
    uint8_t character_id, run_state, jump_state, flap_state, move_direction;
    float pos_X, pos_Y, velocity_Y;
    receive_single_int(character_id);
    receive_single_float(pos_X);
    receive_single_float(pos_Y);
    receive_single_int(run_state);
    receive_single_int(jump_state);
    receive_single_int(flap_state);
    receive_single_int(move_direction);
    receive_single_float(velocity_Y);
    received = Gamestate(character_id, pos_X, pos_Y, run_state, jump_state, flap_state, move_direction, velocity_Y);
}

// bool ClientProtocol::theres_more_data_per_code(int msg_code) {
//     return (msg_code == PICKUP_MESSAGE_CODE) ? true : false;
// }

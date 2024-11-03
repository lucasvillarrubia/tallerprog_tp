#include "client_protocol.h"

#include <utility>


ClientProtocol::ClientProtocol(Socket&& skt, std::atomic_bool& connection_status): Protocol(std::move(skt), connection_status) {}

void ClientProtocol::send_message(const Gameaction& message)
{
    if (client_disconnected.load()) return;
    send_single_int_message(message.player_id);
    send_single_int_message(message.type);
    send_single_int_message(message.key);
}

void ClientProtocol::receive_message(Gamestate& received)
{
    if (client_disconnected.load()) return;
    uint8_t player, type, key;
    // velocity_int, velocity_dec;
    // float velocity_Y;
    receive_single_int_message(player);
    receive_single_int_message(type);
    receive_single_int_message(key);
    // receive_single_int_message(velocity_int);
    // receive_single_int_message(velocity_dec);
    received = Gamestate(player, type, key);
    // received = Gamestate(player, type, key, velocity_Y);
}

// bool ClientProtocol::theres_more_data_per_code(int msg_code) {
//     return (msg_code == PICKUP_MESSAGE_CODE) ? true : false;
// }

#include "server_protocol.h"

#include <utility>


ServerProtocol::ServerProtocol(Socket&& skt, std::atomic_bool& connection_status): Protocol(std::move(skt), connection_status) {}

void ServerProtocol::send_message(const Gamedata&) {}

Gamedata ServerProtocol::receive_message() {}

// EJEMPLOS PARA OVERLOADS DE SEND Y RECEIVE (del tp de threads):
// Command ServerProtocol::receive_message() {
//     if (client_disconnected.load())
//         return Command();
//     uint8_t header, box_id;
//     std::vector<char> player_name;
//     receive_single_int_message(header);
//     receive_string_message(player_name);
//     receive_single_int_message(box_id);
//     return Command(player_name, box_id);
// }
//
// void ServerProtocol::send_pickup_message(int reward_id, const std::vector<char>& player_name) {
//     if (client_disconnected.load())
//         return;
//     uint8_t reward = reward_id;
//     send_single_int_message(MESSAGE_SENDING_HEADER);
//     send_single_int_message(PICKUP_MSG_CODE);
//     send_string_message(player_name);
//     send_single_int_message(reward);
// }

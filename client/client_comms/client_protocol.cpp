#include "client_protocol.h"

#include <utility>


ClientProtocol::ClientProtocol(Socket&& skt, std::atomic_bool& connection_status): Protocol(std::move(skt), connection_status) {}

void ClientProtocol::send_message() {}

void ClientProtocol::receive_message() {}

// bool ClientProtocol::theres_more_data_per_code(int msg_code) {
//     return (msg_code == PICKUP_MESSAGE_CODE) ? true : false;
// }

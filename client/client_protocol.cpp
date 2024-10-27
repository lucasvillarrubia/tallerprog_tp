#include "client_protocol.h"

#include <string>
#include <utility>

#include <arpa/inet.h>

#include "client_exceptions.h"


const char END_OF_MESSAGE = '\0';
const int MESSAGE_HEADER = 0x03;
const int PICKUP_MESSAGE_CODE = 0x04;


ClientProtocol::ClientProtocol(Socket&& client_peer): peer(std::move(client_peer)) {}

Message ClientProtocol::receive_message() {
    uint8_t header, code;
    receive_single_int_message(header);
    receive_single_int_message(code);
    if (theres_more_data_per_code(code)) {
        std::vector<char> player_name;
        uint8_t reward_id;
        receive_string_message(player_name);
        receive_single_int_message(reward_id);
        return Message(player_name, reward_id);
    } else {
        return Message();
    }
}

void ClientProtocol::send_message(int box_id, const std::vector<char>& player_name) {
    send_single_int_message(MESSAGE_HEADER);
    send_string_message(player_name);
    send_single_int_message(box_id);
}

void ClientProtocol::receive_string_message(std::vector<char>& message) {
    bool server_was_disconnected = false;
    uint16_t message_size = 0;
    peer.recvall(&message_size, sizeof(uint16_t), &server_was_disconnected);
    if (server_was_disconnected) {
        message.clear();
        throw ClosedSocket();
    }
    message_size = ntohs(message_size);
    std::vector<char> from_server(message_size);
    peer.recvall(from_server.data(), message_size, &server_was_disconnected);
    if (server_was_disconnected) {
        message.clear();
        throw ClosedSocket();
    }
    from_server.push_back(END_OF_MESSAGE);
    message.resize(from_server.size());
    message = from_server;
}

void ClientProtocol::receive_single_int_message(uint8_t& message) {
    bool server_was_disconnected = false;
    peer.recvall(&message, sizeof(uint8_t), &server_was_disconnected);
    if (server_was_disconnected)
        throw ClosedSocket();
}

void ClientProtocol::send_string_message(const std::vector<char>& message) {
    bool server_was_disconnected = false;
    uint16_t message_size = htons(message.size());
    peer.sendall(&message_size, sizeof(uint16_t), &server_was_disconnected);
    if (server_was_disconnected)
        throw ClosedSocket();
    message_size = ntohs(message_size);
    peer.sendall(message.data(), message_size, &server_was_disconnected);
}

void ClientProtocol::send_single_int_message(uint8_t message) {
    bool server_was_disconnected = false;
    peer.sendall(&message, sizeof(uint8_t), &server_was_disconnected);
    if (server_was_disconnected)
        throw ClosedSocket();
}

bool ClientProtocol::theres_more_data_per_code(int msg_code) {
    return (msg_code == PICKUP_MESSAGE_CODE) ? true : false;
}

ClientProtocol::~ClientProtocol() {}

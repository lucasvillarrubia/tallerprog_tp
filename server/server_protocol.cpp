#include "server_protocol.h"

#include <iostream>
#include <utility>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "../common/liberror.h"


const char END_OF_MESSAGE = '\0';
const uint8_t MESSAGE_SENDING_HEADER = 0x06;
const uint8_t BOX_APPEARENCE_MSG_CODE = 0x05;
const uint8_t PICKUP_MSG_CODE = 0x04;


ServerProtocol::ServerProtocol(Socket&& skt, std::atomic_bool& connection_status):
        peer(std::move(skt)), client_disconnected(connection_status) {}

Command ServerProtocol::receive_message() {
    if (client_disconnected.load())
        return Command();
    uint8_t header, box_id;
    std::vector<char> player_name;
    receive_single_int_message(header);
    receive_string_message(player_name);
    receive_single_int_message(box_id);
    return Command(player_name, box_id);
}

void ServerProtocol::send_pickup_message(int reward_id, const std::vector<char>& player_name) {
    if (client_disconnected.load())
        return;
    uint8_t reward = reward_id;
    send_single_int_message(MESSAGE_SENDING_HEADER);
    send_single_int_message(PICKUP_MSG_CODE);
    send_string_message(player_name);
    send_single_int_message(reward);
}

void ServerProtocol::send_box_appearence_message() {
    if (client_disconnected.load())
        return;
    send_single_int_message(MESSAGE_SENDING_HEADER);
    send_single_int_message(BOX_APPEARENCE_MSG_CODE);
}

void ServerProtocol::receive_single_int_message(uint8_t& message) {
    if (client_disconnected.load())
        return;
    bool client_was_disconnected = false;
    peer.recvall(&message, sizeof(uint8_t), &client_was_disconnected);
    if (client_was_disconnected)
        client_disconnected.store(true);
}

void ServerProtocol::receive_string_message(std::vector<char>& message) {
    if (client_disconnected.load())
        return;
    bool client_was_disconnected = false;
    uint16_t message_size = 0;
    peer.recvall(&message_size, sizeof(uint16_t), &client_was_disconnected);
    if (client_was_disconnected) {
        message.clear();
        client_disconnected.store(true);
        return;
    }
    message_size = ntohs(message_size);
    std::vector<char> from_server(message_size);
    peer.recvall(from_server.data(), message_size, &client_was_disconnected);
    if (client_was_disconnected) {
        message.clear();
        client_disconnected.store(true);
        return;
    }
    from_server.push_back(END_OF_MESSAGE);
    message.resize(from_server.size());
    message = from_server;
}

void ServerProtocol::send_single_int_message(uint8_t message) {
    if (client_disconnected.load())
        return;
    bool client_was_disconnected = false;
    peer.sendall(&message, sizeof(uint8_t), &client_was_disconnected);
    if (client_was_disconnected)
        client_disconnected.store(true);
}

void ServerProtocol::send_string_message(const std::vector<char>& message) {
    if (client_disconnected.load())
        return;
    bool client_was_disconnected = false;
    uint16_t message_size = htons(message.size());
    peer.sendall(&message_size, sizeof(uint16_t), &client_was_disconnected);
    if (client_was_disconnected) {
        client_disconnected.store(true);
        return;
    }
    message_size = ntohs(message_size);
    peer.sendall(message.data(), message_size, &client_was_disconnected);
    if (client_was_disconnected)
        client_disconnected.store(true);
}

void ServerProtocol::close_comms() {
    std::unique_lock<std::mutex> lck(mtx);
    try {
        peer.shutdown(SHUT_RDWR);
        peer.close();
    } catch (const LibError& e) {
        std::cerr << "Se cerró el socket del lado del servidor.\n";
    }
    client_disconnected.store(true);
}

ServerProtocol::~ServerProtocol() {}

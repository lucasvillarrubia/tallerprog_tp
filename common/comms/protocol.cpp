#include "protocol.h"

#include <cmath>

#include "common/hands_on_sockets/liberror.h"

#include <iostream>
#include <utility>

#include <arpa/inet.h>
#include <sys/socket.h>


const char END_OF_MESSAGE = '\0';


Protocol::Protocol(Socket&& skt, std::atomic_bool& connection_status):
        peer(std::move(skt)), client_is_connected(connection_status) {}

void Protocol::receive_single_int(uint8_t& message) {
    if (not client_is_connected.load())
        return;
    bool client_was_disconnected = false;
    peer.recvall(&message, sizeof(uint8_t), &client_was_disconnected);
    if (client_was_disconnected)
        client_is_connected.store(false);
}

void Protocol::receive_string(std::vector<char>& message) {
    if (not client_is_connected.load())
        return;
    bool client_was_disconnected = false;
    uint16_t message_size = 0;
    peer.recvall(&message_size, sizeof(uint16_t), &client_was_disconnected);
    if (client_was_disconnected) {
        message.clear();
        client_is_connected.store(false);
        return;
    }
    message_size = ntohs(message_size);
    std::vector<char> from_server(message_size);
    peer.recvall(from_server.data(), message_size, &client_was_disconnected);
    if (client_was_disconnected) {
        message.clear();
        client_is_connected.store(false);
        return;
    }
    from_server.push_back(END_OF_MESSAGE);
    message.resize(from_server.size());
    message = from_server;
}

void Protocol::send_single_int(uint8_t message) {
    if (not client_is_connected.load())
        return;
    bool client_was_disconnected = false;
    peer.sendall(&message, sizeof(uint8_t), &client_was_disconnected);
    if (client_was_disconnected)
        client_is_connected.store(false);
}

void Protocol::send_string(const std::vector<char>& message) {
    if (not client_is_connected.load())
        return;
    bool client_was_disconnected = false;
    uint16_t message_size = htons(message.size());
    peer.sendall(&message_size, sizeof(uint16_t), &client_was_disconnected);
    if (client_was_disconnected) {
        client_is_connected.store(false);
        return;
    }
    message_size = ntohs(message_size);
    peer.sendall(message.data(), message_size, &client_was_disconnected);
    if (client_was_disconnected)
        client_is_connected.store(false);
}

void Protocol::receive_single_float(float& message) {
    uint8_t integer, decimal;
    receive_single_int(integer);
    receive_single_int(decimal);
    float fraction_part = decimal / 100.0f;
    message = integer + fraction_part;
}

void Protocol::send_single_float(float message) {
    float fraction_part = std::modf(message, &message);
    uint8_t integer = static_cast<uint8_t>(message);
    uint8_t decimal = static_cast<uint8_t>(fraction_part * 100);
    send_single_int(integer);
    send_single_int(decimal);
}

void Protocol::close_comms() {
    std::unique_lock<std::mutex> lck(mtx);
    try {
        peer.shutdown(SHUT_RDWR);
        peer.close();
    } catch (const LibError& e) {
        std::cerr << "Se cerró el socket de un protocolo.\n";
    }
    client_is_connected.store(false);
}

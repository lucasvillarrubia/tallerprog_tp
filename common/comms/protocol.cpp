#include "protocol.h"

#include "common/hands_on_sockets/liberror.h"

#include <iostream>
#include <utility>

#include <arpa/inet.h>
#include <sys/socket.h>


const char END_OF_MESSAGE = '\0';


Protocol::Protocol(Socket&& skt, std::atomic_bool& connection_status):
        peer(std::move(skt)), client_disconnected(connection_status) {}

void Protocol::receive_single_int_message(uint8_t& message) {
    if (client_disconnected.load())
        return;
    bool client_was_disconnected = false;
    peer.recvall(&message, sizeof(uint8_t), &client_was_disconnected);
    if (client_was_disconnected)
        client_disconnected.store(true);
}

void Protocol::receive_string_message(std::vector<char>& message) {
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

void Protocol::send_single_int_message(uint8_t message) {
    if (client_disconnected.load())
        return;
    bool client_was_disconnected = false;
    peer.sendall(&message, sizeof(uint8_t), &client_was_disconnected);
    if (client_was_disconnected)
        client_disconnected.store(true);
}

void Protocol::send_string_message(const std::vector<char>& message) {
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

void Protocol::close_comms() {
    std::unique_lock<std::mutex> lck(mtx);
    try {
        peer.shutdown(SHUT_RDWR);
        peer.close();
    } catch (const LibError& e) {
        std::cerr << "Se cerró el socket de un protocolo.\n";
    }
    client_disconnected.store(true);
}

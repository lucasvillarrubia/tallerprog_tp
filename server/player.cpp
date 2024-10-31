#include <iostream>
#include <utility>

#include "common/hands_on_sockets/liberror.h"

#include "player.h"


Player::Player(Socket&& skt, Queue<Command>& usr_entr):
        server_messages(usr_entr),
        client_is_disconnected(true),
        protocol(std::move(skt), client_is_disconnected),
        sender(protocol, client_is_disconnected),
        receiver(protocol, client_is_disconnected) {}

void Player::start() {
    try {
        client_is_disconnected.store(false);
        receiver.start();
        sender.start();
    } catch (const LibError& e) {
        std::cerr << "Liberror en el player!\n";
        client_is_disconnected.store(true);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    } catch (...) {
        std::cerr << "Unknown exception on a player.\n";
    }
}

void Player::add_message_to_queue(const Command& to_send) {
    if (not client_is_disconnected.load()) {
        messages_queue.try_push(to_send);
    }
}

bool Player::is_connected() { return !client_is_disconnected.load(); }

void Player::disconnect() {
    client_is_disconnected.store(true);
    messages_queue.close();
    protocol.close_comms();
    receiver.stop();
    receiver.join();
    sender.stop();
    sender.join();
}

Player::~Player() {}

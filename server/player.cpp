#include <iostream>
#include <utility>

#include "common/hands_on_sockets/liberror.h"

#include "player.h"


Player::Player(Socket&& skt, Queue<Gameaction>& usr_entr):
        id(0),
        server_messages(usr_entr),
        client_is_connected(false),
        protocol(std::move(skt), client_is_connected),
        sender(client_is_connected, protocol, messages_queue),
        receiver(id, client_is_connected, protocol, server_messages) {}

void Player::start() {
    try {
        client_is_connected.store(true);
        receiver.start();
        sender.start();
    } catch (const LibError& e) {
        std::cerr << "Liberror en el player!\n";
        client_is_connected.store(false);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    } catch (...) {
        std::cerr << "Unknown exception on a player.\n";
    }
}

void Player::add_message_to_queue(const Gamestate& to_send) {
    if (client_is_connected.load()) {
        if (to_send.type == 11) {
            std::cout << "sending message to player " << to_send.player_id << '\n';
            std::cout << "match id: " << to_send.match_id << '\n';
        }
        messages_queue.try_push(to_send);
    }
}

bool Player::is_connected() { return client_is_connected.load(); }

bool Player::matches(int id) const
{
    return this->id == id;
}

void Player::disconnect() {
    client_is_connected.store(false);
    messages_queue.close();
    protocol.close_comms();
    receiver.stop();
    receiver.join();
    sender.stop();
    sender.join();
}

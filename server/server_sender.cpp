#include <iostream>

#include "../common/liberror.h"

#include "server_sender.h"


Sender::Sender(ServerProtocol& srv_prot, Queue<Command>& srv_msgs,
               std::atomic_bool& connection_status):
        is_running(true),
        protocol(srv_prot),
        server_messages(srv_msgs),
        client_disconnected(connection_status) {}

void Sender::run() {
    while (is_running.load() and not client_disconnected.load()) {
        try {
            Command message_data = server_messages.pop();
            if (not message_data.player.empty()) {
                message_data.player.pop_back();
                protocol.send_pickup_message(message_data.id, message_data.player);
                if (client_disconnected.load())
                    break;
            } else {
                protocol.send_box_appearence_message();
                if (client_disconnected.load())
                    break;
            }
        } catch (ClosedQueue const&) {
        } catch (const LibError& e) {
            std::cerr << "Liberror en el sender!\n";
            throw;
        } catch (const std::exception& e) {
            std::cerr << "Exception caught in the sender:" << e.what() << '\n';
            stop();
        } catch (...) {
            std::cerr << "Unknown exception on a player's sender.\n";
            stop();
        }
    }
}

void Sender::stop() {
    _keep_running = false;
    is_running.store(false);
}

Sender::~Sender() {}

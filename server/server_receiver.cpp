#include <iostream>

#include "../common/liberror.h"

#include "server_receiver.h"


Receiver::Receiver(ServerProtocol& srv_prot, Queue<Command>& usr_entr,
                   std::atomic_bool& connection_status):
        is_running(true),
        protocol(srv_prot),
        user_entries(usr_entr),
        client_disconnected(connection_status) {}

void Receiver::run() {
    while (is_running.load() and not client_disconnected.load()) {
        try {
            Command received = protocol.receive_message();
            if (client_disconnected.load()) {
                break;
            }
            user_entries.push(received);
        } catch (ClosedQueue const&) {
        } catch (const LibError& e) {
            std::cerr << "Liberror en el receiver!\n";
            throw;
        } catch (const std::exception& e) {
            std::cerr << "Exception caught in the receiver:" << e.what() << '\n';
            stop();
        } catch (...) {
            std::cerr << "Unknown exception on a player's receiver.\n";
            stop();
        }
    }
}

void Receiver::stop() {
    _keep_running = false;
    is_running.store(false);
}

Receiver::~Receiver() {}

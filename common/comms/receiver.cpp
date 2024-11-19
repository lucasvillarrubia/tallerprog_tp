#include "receiver.h"

#include "common/hands_on_sockets/liberror.h"
#include "common/hands_on_threads/queue.h"

#include <iostream>


Receiver::Receiver(std::atomic_bool& connection_status):
        is_running(false),
        client_is_connected(connection_status) {}

void Receiver::run() {
    is_running.store(true);
    while (is_running.load() and client_is_connected.load()) {
        try {
            receive_data();
        } catch (ClosedQueue const&) {
        } catch (const LibError& e) {
            std::cerr << "Liberror en un receiver!\n";
            throw;
        } catch (const std::exception& e) {
            std::cerr << "Exception caught in a receiver:" << e.what() << '\n';
            stop();
        } catch (...) {
            std::cerr << "Unknown exception on a receiver.\n";
            stop();
        }
    }
}

void Receiver::stop() {
    _keep_running = false;
    is_running.store(false);
}

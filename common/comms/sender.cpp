#include "sender.h"

#include "common/hands_on_sockets/liberror.h"
#include "common/hands_on_threads/queue.h"

#include <iostream>


Sender::Sender(std::atomic_bool& connection_status):
        is_running(false),
        client_disconnected(connection_status) {}

void Sender::run() {
    is_running.store(true);
    while (is_running.load() and not client_disconnected.load()) {
        try {
            send_data();
        } catch (ClosedQueue const&) {
        } catch (const LibError& e) {
            std::cerr << "Liberror en un sender!\n";
            throw;
        } catch (const std::exception& e) {
            std::cerr << "Exception caught in a sender:" << e.what() << '\n';
            stop();
        } catch (...) {
            std::cerr << "Unknown exception on a sender.\n";
            stop();
        }
    }
}

void Sender::stop() {
    _keep_running = false;
    is_running.store(false);
}

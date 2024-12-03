#include "client_receiver.h"


ClientReceiver::ClientReceiver(std::atomic_bool& cli_stat, ClientProtocol& ptc, Queue<Gamestate>& updates, StateManager& state):
        Receiver(cli_stat),
        protocol(ptc),
        server_updates(updates),
        state(state) {}

void ClientReceiver::receive_data()
{
    Gamestate received_state;
    protocol.receive_message(received_state);
    if (client_is_connected.load()) {
        server_updates.push(received_state);
    }
}



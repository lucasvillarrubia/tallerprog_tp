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
        if (received_state.type == 11) {
                std::cout << "received message from server with match id " << received_state.match_id << '\n';
        }
        // if (received_state.type >= 10 and received_state.type <= 12) {
        //     server_updates.push(received_state);
        // }
        // else {
        //     state.update(received_state);
        // }
        server_updates.push(received_state);
    }
}



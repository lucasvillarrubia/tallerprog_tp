#include "server_receiver.h"


ServerReceiver::ServerReceiver(const int& _id, std::atomic_bool& con_stat, ServerProtocol& srv_prot, Queue<Gameaction>& q):
        Receiver(con_stat),
        id(_id),
        protocol(srv_prot),
        clients_actions(q) {}

void ServerReceiver::receive_data()
{
    Gameaction received_event;
    protocol.receive_message(received_event);
    if (client_is_connected.load()) {
        received_event.player_id = id;
        clients_actions.push(received_event);
    }
}

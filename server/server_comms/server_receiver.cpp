#include "server_receiver.h"


ServerReceiver::ServerReceiver(std::atomic_bool& con_stat, ServerProtocol& srv_prot, Queue<Gameaction>& q):
        Receiver(con_stat),
        protocol(srv_prot),
        clients_actions(q) {}

void ServerReceiver::receive_data()
{
    Gameaction received_event = static_cast<Gameaction>(protocol.receive_message());
    if (not client_disconnected.load())
        clients_actions.push(received_event);
}

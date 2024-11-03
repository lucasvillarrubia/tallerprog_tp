#include "server_sender.h"


ServerSender::ServerSender(std::atomic_bool& con_stat, ServerProtocol& srv_prot, Queue<Gamestate>& q): Sender(con_stat), protocol(srv_prot), server_messages(q) {}

void ServerSender::send_data()
{
    Gamestate new_event = server_messages.pop();
    if (not client_disconnected.load())
        protocol.send_message(new_event);
}



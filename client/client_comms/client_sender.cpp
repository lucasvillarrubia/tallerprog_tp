#include "client_sender.h"


ClientSender::ClientSender(std::atomic_bool& cli_stat, ClientProtocol& ptc, Queue<Gameaction>& events): Sender(cli_stat), protocol(ptc), player_actions(events) {}

void ClientSender::send_data()
{
    Gameaction new_event = player_actions.pop();
    if (not client_disconnected.load())
        protocol.send_message(new_event);
}

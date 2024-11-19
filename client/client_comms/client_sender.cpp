#include "client_sender.h"


ClientSender::ClientSender(std::atomic_bool& cli_stat, ClientProtocol& ptc, Queue<Gameaction>& events): Sender(cli_stat), protocol(ptc), player_actions(events) {}

void ClientSender::send_data()
{
    Gameaction new_event = player_actions.pop();
    // std::cout << "pude sacar un evento de la queue en el sender!\n";
    // std::cout << "el cliente está conectado?: " << (client_is_connected.load() ? "SI\n" : "NO\n");
    if (client_is_connected.load()) {
        protocol.send_message(new_event);
        // std::cout << "mandé un evento desde el cliente\n";
    }
}

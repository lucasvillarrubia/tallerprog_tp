#include "client_sender.h"


ClientSender::ClientSender(std::atomic_bool& cli_stat, ClientProtocol& ptc, Queue<Gameaction>& events): Sender(cli_stat), protocol(ptc), player_actions(events) {}

void ClientSender::send_data() {}

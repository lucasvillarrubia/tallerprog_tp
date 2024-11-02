#include "client_receiver.h"


ClientReceiver::ClientReceiver(std::atomic_bool& cli_stat, ClientProtocol& ptc, Queue<Gamestate>& updates): Receiver(cli_stat), protocol(ptc), server_updates(updates) {}

void ClientReceiver::receive_data() {}



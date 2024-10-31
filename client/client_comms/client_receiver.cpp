#include "client_receiver.h"


ClientReceiver::ClientReceiver(ClientProtocol& ptc, std::atomic_bool& cli_stat): Receiver(cli_stat), protocol(ptc) {}

void ClientReceiver::receive_data() {}



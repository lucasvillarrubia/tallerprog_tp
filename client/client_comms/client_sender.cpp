#include "client_sender.h"


ClientSender::ClientSender(ClientProtocol& ptc, std::atomic_bool& cli_stat): Sender(cli_stat), protocol(ptc) {}

void ClientSender::send_data() {}

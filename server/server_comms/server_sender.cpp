#include "server_sender.h"


ServerSender::ServerSender(ServerProtocol& srv_prot, std::atomic_bool& con_stat): Sender(con_stat), protocol(srv_prot) {}

void ServerSender::send_data() {}



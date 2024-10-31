#include "server_receiver.h"


ServerReceiver::ServerReceiver(ServerProtocol& srv_prot, std::atomic_bool& con_stat): Receiver(con_stat), protocol(srv_prot) {}

void ServerReceiver::receive_data() {}

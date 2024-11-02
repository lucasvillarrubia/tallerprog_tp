#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H


#include "common/comms/protocol.h"
#include "common/hands_on_sockets/socket.h"


// Clase de protocolo de servidor. Pensada para ser de un único cliente.
// Maneja (y posee) el Socket en el que se comparten los enviós
// y recibimientos (con los dos streams que tiene el Socket).
// Es compartido por el Sender y el Receiver!
// RAII: no permite copia.


class ServerProtocol: public Protocol
{
public:
    ServerProtocol(Socket&&, std::atomic_bool&);
    void send_message(const Gamedata&) override;
    Gamedata receive_message() override;
    ~ServerProtocol() override = default;
};


#endif
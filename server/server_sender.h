#ifndef SENDER_H
#define SENDER_H

#include <atomic>

#include "../common/queue.h"
#include "../common/thread.h"

#include "command.h"
#include "server_protocol.h"


// Al igual que el Receiver, hereda de Thread.
// Comparte:
// - con otros Senders, la queue de mensajes del servidor para enviar.
// - con el Receiver del mismo cliente conectado, la referencia al protocolo del cliente.
// Envía desde la queue de mensajes del servidor (con un pop) los mensajes requeridos al (único)
// cliente que escucha.
// RAII: hereda de Thread (no permite copia ni movimiento).


class Sender: public Thread {
private:
    std::atomic_bool is_running;
    ServerProtocol& protocol;
    Queue<Command>& server_messages;
    std::atomic_bool& client_disconnected;

public:
    Sender(ServerProtocol&, Queue<Command>&, std::atomic_bool&);
    void run() override;
    void stop() override;
    ~Sender();
};


#endif
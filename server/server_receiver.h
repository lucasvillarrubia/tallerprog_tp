#ifndef RECEIVER_H
#define RECEIVER_H

#include <atomic>

#include "../common/message.h"
#include "../common/queue.h"
#include "../common/thread.h"

#include "command.h"
#include "server_protocol.h"


// Objeto activo (hereda de Thread) que encapsula el hilo recibidor para el servidor.
// Recibe el único tipo de mensaje del único cliente al que escucha.
// Pushea los comandos a la queue de mensajes.
// Comparte:
// - con el servidor, el gameplay y los otros Receivers, la queue de comandos ingresados por los
// usuarios.
// - con el Sender del mismo cliente, la referencia al protocolo del cliente.
// RAII: hereda de Thread (no permite copia ni movimiento).


class Receiver: public Thread {
private:
    std::atomic_bool is_running;
    ServerProtocol& protocol;
    Queue<Command>& user_entries;
    std::atomic_bool& client_disconnected;

public:
    Receiver(ServerProtocol&, Queue<Command>&, std::atomic_bool&);
    void run() override;
    void stop() override;
    ~Receiver();
};


#endif
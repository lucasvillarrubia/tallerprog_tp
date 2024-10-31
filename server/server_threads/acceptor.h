#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <atomic>

#include "common/hands_on_sockets/socket.h"
#include "common/hands_on_threads/queue.h"
#include "common/hands_on_threads/thread.h"
#include "server/command.h"
#include "server/monitored_list.h"


// Objeto activo que representa al hilo aceptador (de clientes).
// Espera clientes de manera asincrónica al funcionamiento del juego.
// Recibe clientes en cualquier momento (los acepta y los pushea a la lista de jugadores).
// Al recibir a los clientes les asigna un único de protocolo para su correspondiente Socket.
// Comparte:
// - con el gameloop, la lista de jugadores conectados.
// RAII: hereda de Thread (no permite copia ni movimiento).


class Acceptor: public Thread {
private:
    std::atomic_bool is_running;
    Socket acceptor;
    MonitoredList& player_list;
    Queue<Command>& user_commands;

public:
    Acceptor(const char*, MonitoredList&, Queue<Command>&);
    void run() override;
    void stop() override;
    ~Acceptor();
};


#endif
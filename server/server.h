#ifndef SERVER_H
#define SERVER_H


#include "common/hands_on_threads/queue.h"

#include "server_threads/acceptor.h"
#include "server_threads/gameplay.h"
#include "monitored_list.h"


// Clase de entidad de servidor.
// Lee la entrada estándar hasta que se ingresa el comando de finalización.
// Ejecuta las acciones del servidor y lanza los hilos aceptador y del loop de juego.
// Posee los recursos que se comparten entre otros threads y conecta la lógica del juego con las
// acciones del multicliente (y se encarga de cerrarlos/liberarlos).
// RAII: no permite copia ni movimiento (contiene Threads).


class Server {
private:
    MonitoredList player_list;
    Queue<Command> user_commands;
    Acceptor acceptor;
    Gameplay gameloop;

public:
    explicit Server(const char*);
    void run();
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    Server(Server&& other) = delete;
    Server& operator=(Server&& other) = delete;
    ~Server();
};


#endif
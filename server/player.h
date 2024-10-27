#ifndef PLAYER_H
#define PLAYER_H


#include "../common/socket.h"

#include "command.h"
#include "server_receiver.h"
#include "server_sender.h"
#include "server_protocol.h"


// Clase que representa un cliente del lado del servidor.
// Lanza los hilos Sender y Receiver correspondientes al cliente. Se puede
// desconectar.
// RAII: no permite copia ni movimiento (contiene Threads).


class Player {
private:
    std::atomic_bool client_is_disconnected;
    ServerProtocol protocol;
    Queue<Command> messages_queue;
    Sender sender;
    Receiver receiver;

public:
    Player(Socket&&, Queue<Command>&);
    void start();
    void add_message_to_queue(const Command&);
    bool is_connected();
    void disconnect();
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    Player(Player&& other) = delete;
    Player& operator=(Player&& other) = delete;
    ~Player();
};


#endif
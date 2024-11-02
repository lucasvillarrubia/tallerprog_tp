#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H


#include "command.h"
#include "client/character.h"

#include "common/hands_on_sockets/socket.h"
#include "common/hands_on_threads/queue.h"
#include "server_comms/server_receiver.h"
#include "server_comms/server_sender.h"
#include "server_comms/server_protocol.h"


// Clase que representa un cliente del lado del servidor.
// Lanza los hilos Sender y Receiver correspondientes al cliente. Se puede
// desconectar.
// RAII: no permite copia ni movimiento (contiene Threads).


class Player
{
private:
    Queue<Command>& server_messages;
    std::atomic_bool client_is_disconnected;
    ServerProtocol protocol;
    Queue<Command> messages_queue;
    ServerSender sender;
    ServerReceiver receiver;
    Character state;
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
    ~Player() = default;
};


#endif
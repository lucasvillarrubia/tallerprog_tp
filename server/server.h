#ifndef SERVER_H
#define SERVER_H


#include "common/hands_on_threads/queue.h"

#include "server_threads/acceptor.h"
#include "server_threads/gameplay.h"
#include "monitored_list.h"
#include "server_threads/cleaner.h"


class Server
{
private:
    // si el servidor va a aguantar varias partidas, un atributo va a ser:
    // MonitoredList<Match> match_list;
    MonitoredList player_list;
    Queue<Command> user_commands;
    Acceptor acceptor;
    Gameplay gameloop;
    Cleaner cleaner;
public:
    explicit Server(const char*);
    void run();
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    Server(Server&& other) = delete;
    Server& operator=(Server&& other) = delete;
    ~Server() = default;
};


#endif
#ifndef SERVER_H
#define SERVER_H


#include "match.h"
#include "common/hands_on_threads/queue.h"

#include "server_threads/acceptor.h"
#include "server_threads/gameplay.h"
#include "monitored_list.h"
#include "server_threads/cleaner.h"
#include "server_threads/match_manager.h"


class Server
{
private:
    // si el servidor va a aguantar varias partidas, un atributo va a ser:
    MonitoredList<Player*> player_list;
    MonitoredList<Match*> match_list;
    Queue<Gameaction> user_commands;
    Acceptor acceptor;
    MatchManager match_creator;
    // cada partida va tener un Gameplay
    // Gameplay gameloop;
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
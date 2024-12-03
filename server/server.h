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
    MonitoredList<Player*> player_list;
    MonitoredList<Match*> match_list;
    Queue<Gameaction> user_commands;
    Acceptor acceptor;
    MatchManager match_creator;
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
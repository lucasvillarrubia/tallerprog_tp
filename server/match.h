#ifndef MATCH_H
#define MATCH_H
#include "server_threads/gameplay.h"


class Match {
private:
    int id;
    int winner_id;
    MonitoredList<Player*> player_list;
    Queue<Gameaction> user_commands;
    Gameplay gameloop;
public:
    explicit Match(int);
    void start();
    void end();
    bool is_connected();
    void add_player(Player*);
    void add_action(const Gameaction&);
    bool matches(int);
    bool has_ended();
    void disconnect();
};



#endif

#ifndef MATCH_H
#define MATCH_H
#include "server_threads/gameplay.h"


class Match {
private:
    //MonitoredList<Player*> player_list;
    //Queue<Gameaction> user_commands;
    //Gameplay gameloop;
public:
    Match();
    void start();
    void end();
    bool is_connected();
    bool matches(int);
    void disconnect();
};



#endif

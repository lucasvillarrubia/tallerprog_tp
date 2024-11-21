#ifndef MATCH_H
#define MATCH_H
#include "server_threads/gameplay.h"


class Match {
private:
    int id;
    int player_limit;
    int winner_id;
    bool has_started;
    MonitoredList<Player*> player_list;
    Queue<Gameaction> user_commands;
    Gameplay gameloop;
public:
    Match(int, int);
    void start();
    void end();
    bool is_connected();
    void add_player(Player*);
    void add_action(const Gameaction&);
    bool matches(int);
    bool has_ended();
    void disconnect();
    int get_player_count() { return player_list.size(); }
    bool is_full() { return player_list.size() == player_limit; }
    Match(const Match&) = delete;
    Match& operator=(const Match&) = delete;
    Match(Match&& other) = delete;
    Match& operator=(Match&& other) = delete;
    ~Match() = default;
};



#endif

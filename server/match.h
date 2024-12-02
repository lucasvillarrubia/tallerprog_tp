#ifndef MATCH_H
#define MATCH_H
#include "server_threads/gameplay.h"


class Match {
private:
    int id;
    int winner_id;
    bool has_started;
    MonitoredList<Player*> player_list;
    Queue<Gameaction> user_commands;
    Gameplay gameloop;
    std::map<int, bool> multiplayer_mode_by_player;
public:
    Match(int);
    void start();
    bool is_connected();
    void send_start_message(int);
    void add_player(Player*, int, bool);
    void add_action(const Gameaction&);
    bool matches(int);
    bool is_player_in_match(int);
    bool has_ended();
    void disconnect();
    void close_queue() { user_commands.close(); }
    int get_player_count() { return player_list.size(); }
    Match(const Match&) = delete;
    Match& operator=(const Match&) = delete;
    Match(Match&& other) = delete;
    Match& operator=(Match&& other) = delete;
    ~Match() = default;
};



#endif

#ifndef MATCH_MANAGER_H
#define MATCH_MANAGER_H
#include "common/hands_on_threads/thread.h"
#include "server/match.h"
#include "server/monitored_list.h"


class MatchManager: public Thread
{
private:
    std::map<int, int> creators_by_match;
    int match_count;
    std::atomic_bool is_running;
    Queue<Gameaction>& users_commands;
    MonitoredList<Player*>& all_players;
    MonitoredList<Match*>& matches;
    // void create_match(int);
    void create_match(int, int);
    void join_to_match(int, int, int);
    void start_match(int, int);
    void add_action_to_match(const Gameaction&, const int);
    void send_matches_info(int);
    void close_match(int, int);
public:
    MatchManager(Queue<Gameaction>&, MonitoredList<Player*>&, MonitoredList<Match*>&);
    void run() override;
    void stop() override;
    ~MatchManager() override = default;
};



#endif

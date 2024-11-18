#ifndef MATCH_MANAGER_H
#define MATCH_MANAGER_H
#include "common/hands_on_threads/thread.h"
#include "server/match.h"
#include "server/monitored_list.h"


class MatchManager: public Thread {
private:
    std::atomic_bool is_running;
    Queue<Gameaction>& users_commands;
    MonitoredList<Player*>& all_players;
    MonitoredList<Match*> matches;
    void create_match(int);
    void join_to_match(int, int);
    void close_match();
public:
    MatchManager(Queue<Gameaction>&, MonitoredList<Player*>&);
    void run() override;
    void stop() override;
    ~MatchManager() override = default;
};



#endif

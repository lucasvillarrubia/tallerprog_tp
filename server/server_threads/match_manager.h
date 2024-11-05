#ifndef MATCH_MANAGER_H
#define MATCH_MANAGER_H
#include "common/hands_on_threads/thread.h"
#include "server/match.h"
#include "server/monitored_list.h"


class MatchManager: public Thread {
private:
    std::atomic_bool is_running;
    MonitoredList<Match*>& matches;
    MonitoredList<Player*>& all_players;
    void start_match();
    void close_match();
public:
    MatchManager(MonitoredList<Match*>&, MonitoredList<Player*>&);
    void run() override;
    void stop() override;
    ~MatchManager() override = default;
};



#endif

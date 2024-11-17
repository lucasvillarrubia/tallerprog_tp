#include "match_manager.h"


MatchManager::MatchManager(Queue<Gameaction>& q, MonitoredList<Player*>& p): is_running(false), users_commands(q), all_players(p) {}

void MatchManager::start_match()
{
    MonitoredList<Player*> new_match_player_list = all_players.select_if();
    auto* new_match = new Match();
    matches.push_back(new_match);
    new_match->start();
}

void MatchManager::close_match()
{
}

void MatchManager::run()
{
    // is_running.store(true);
    // while (is_running.load()) {
    //     Gameaction action = users_commands.pop();
    // }
}

void MatchManager::stop()
{
    Thread::stop();
}

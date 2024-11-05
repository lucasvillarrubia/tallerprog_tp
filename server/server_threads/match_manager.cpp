#include "match_manager.h"


MatchManager::MatchManager(MonitoredList<Match>& m, MonitoredList<Player>& p): is_running(false), matches(m), players(p) {}

void MatchManager::start_match()
{
    MonitoredList<Player*> new_match_player_list = players.select_if();
    Match* new_match = new Match();
    matches.push_back(new_match);
    new_match->start();
}

void MatchManager::close_match()
{
}

void MatchManager::run()
{
    is_running.store(true);
    while (is_running.load()) {
        if (true) {     //condicion para que empiece una partida
            start_match();
            matches.clean_up();
        }
    }
}

void MatchManager::stop()
{
    Thread::stop();
}

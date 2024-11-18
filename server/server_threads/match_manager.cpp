#include "match_manager.h"


MatchManager::MatchManager(Queue<Gameaction>& q, MonitoredList<Player*>& p):
        is_running(false),
        users_commands(q),
        all_players(p) {}

void MatchManager::create_match(int creator_id)
{
    auto* new_match = new Match();
    Player* new_match_player_list = all_players.get_by_id(creator_id);
    new_match->add_player(new_match_player_list);
    matches.push_back(new_match);
}

void MatchManager::join_to_match(int player, int match_id)
{
    auto* match = matches.get_by_id(match_id);
    Player* player_to_join = all_players.get_by_id(player);
    match->add_player(player_to_join);
}

void MatchManager::close_match()
{
}

void MatchManager::run()
{
    is_running.store(true);
    while (is_running.load()) {
        Gameaction action;
        while (users_commands.try_pop(action)) {
            switch (action.type) {
                case 4:
                    create_match(action.player_id);
                    break;
                case 5:
                    join_to_match(action.player_id, action.key);
                    break;
                default:
                    break;
            }
        }
    }
}

void MatchManager::stop()
{
    Thread::stop();
}

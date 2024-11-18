#include "match_manager.h"


MatchManager::MatchManager(Queue<Gameaction>& q, MonitoredList<Player*>& p):
        match_count(0),
        is_running(false),
        users_commands(q),
        all_players(p)
{}

void MatchManager::create_match(int creator_id)
{
    match_count++;
    auto* new_match = new Match(match_count);
    auto* new_match_player_list = all_players.get_by_id(creator_id);
    new_match->add_player(new_match_player_list);
    matches.push_back(new_match);
    creators_by_match.insert({match_count, creator_id});
}

void MatchManager::join_to_match(int player, int match_id)
{
    auto* player_to_join = all_players.get_by_id(player);
    auto* match = matches.get_by_id(match_id);
    if (match == nullptr) {
        Gamestate error(player, "Match not found");
        player_to_join->add_message_to_queue(error);
        return;
    } else if (match->is_connected()) {
        Gamestate error(player, "Match not available");
        player_to_join->add_message_to_queue(error);
        return;
    }
    match->add_player(player_to_join);
}

void MatchManager::start_match(int player, int match_id)
{
    auto* requestor = all_players.get_by_id(player);
    auto* match = matches.get_by_id(match_id);
    if (match == nullptr) {
        Gamestate error(player, "Match not found");
        requestor->add_message_to_queue(error);
        return;
    } else if (creators_by_match.at(match_id) != player) {
        Gamestate error(player, "You cannot start this match");
        requestor->add_message_to_queue(error);
        return;
    }
    match->start();
}

void MatchManager::add_action_to_match(const Gameaction& action)
{
    if (not matches.contains(action.player_id))
        return;
    auto* match = matches.select_one_if(
        [action](Match* m) {
            return m->matches(action.player_id);
        }
    );
    match->add_action(action);
}

void MatchManager::close_match()
{
}

void MatchManager::run()
{
    try {
        is_running.store(true);
        while (is_running.load()) {
            Gameaction action = users_commands.pop();
            // Gameaction action;
            // while (users_commands.try_pop(action)) {
                switch (action.type) {
                    case 4:
                        create_match(action.player_id);
                        break;
                    case 5:
                        join_to_match(action.player_id, action.key);
                        break;
                    case 6:
                        std::cout << "intenté empezar una partida, jugador: " << action.player_id << " y partida: " << action.match << "\n";
                        start_match(action.player_id, action.match);
                        break;
                    default:
                        add_action_to_match(action);
                        break;
                }
            // }
            auto* match = matches.get_by_id(1);
            if (match != nullptr) {
                if (match->has_ended()) {
                    match->disconnect();
                    delete match;
                }

            }
            // matches.remove_if(
            //     [](auto match) {
            //         if (not match->is_connected()) {
            //             match->disconnect();
            //             delete match;
            //             return true;
            //         }
            //         return false;
            //     }
            // );
        }
    } catch (const ClosedQueue& e) {
        std::cerr << "Se cerró la queue en en match manager!\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    } catch (...) {
        std::cerr << "Unknown exception on match manager.\n";
    }
}

void MatchManager::stop()
{
    is_running.store(false);
    matches.clear();
    Thread::stop();
}

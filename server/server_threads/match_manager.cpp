#include "match_manager.h"
#include "common/hands_on_sockets/liberror.h"


const int ONE_PLAYER_LIMIT = 1;
const int TWO_PLAYER_LIMIT = 2;
const int THREE_PLAYER_LIMIT = 3;


MatchManager::MatchManager(Queue<Gameaction>& q, MonitoredList<Player*>& p):
        match_count(0),
        is_running(false),
        users_commands(q),
        all_players(p)
{}

void MatchManager::create_match(int creator_id, int player_limit)
{
    match_count++;
    auto* new_match = new Match(match_count, player_limit);
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
    } else if (match->is_full()) {
        Gamestate error(player, "Match is full");
        player_to_join->add_message_to_queue(error);
        return;
    }
    player_to_join->set_id(match->get_player_count() + 1);
    match->add_player(player_to_join);
    if (match->is_full()) {
        match->start();
    }
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
    if (not match->is_connected())
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
                        create_match(action.player_id, ONE_PLAYER_LIMIT);
                        start_match(action.player_id, 1);
                        break;
                    case 5:
                        join_to_match(action.player_id, action.key);
                        break;
                    case 6:
                        start_match(action.player_id, action.match);
                        break;
                    case 7:
                        create_match(action.player_id, TWO_PLAYER_LIMIT);
                        break;
                    case 8:
                        create_match(action.player_id, THREE_PLAYER_LIMIT);
                        break;
                    default:
                        add_action_to_match(action);
                        break;
                }
            // }
            auto* match = matches.get_by_id(1);
            if (match != nullptr) {
                if (match->has_ended()) {
                    // match->disconnect();
                    // delete match;
                    is_running.store(false);
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
        is_running.store(false);
        _keep_running = false;
        std::cerr << "Se cerró la queue en el match manager!\n";
        // matches.clear();
    } catch (const LibError& e) {
        is_running.store(false);
    } catch (const std::exception& e) {
        is_running.store(false);
        std::cerr << e.what() << '\n';
    } catch (...) {
        is_running.store(false);
        std::cerr << "Unknown exception on match manager.\n";
    }
}

void MatchManager::stop()
{
    try
    {
        if (not is_running.load()) {
            return;
        }
        _keep_running = false;
        is_running.store(false);
        matches.clear();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << " cuando quiero stoppear match_creator\n";
    }
    
    
}

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

void MatchManager::create_match(int creator_id, int creator_multiplayer_mode)
{
    match_count++;
    Match* new_match = new Match(match_count);
    Player* creator = all_players.get_by_id(creator_id);
    new_match->add_player(creator, creator_id, (creator_multiplayer_mode == 1));
    matches.push_back(new_match);
    creators_by_match.insert({match_count, creator_id});
    Gamestate match_created(creator_id, 0, match_count);
    creator->add_message_to_queue(match_created);
    std::cout << "the player " << creator_id << " created a match with id " << match_count << '\n';
}

void MatchManager::join_to_match(int player, int match_id)
{
    auto* player_to_join = all_players.get_by_id(player);
    auto* match = matches.get_by_id(match_id);
    if (match == nullptr) {
        Gamestate error(player, 1, "Match not found");
        player_to_join->add_message_to_queue(error);
        return;
    } else if (match->is_connected()) {
        Gamestate error(player, 1, "Match not available");
        player_to_join->add_message_to_queue(error);
        return;
    }
    match->add_player(player_to_join, player, false);
    Gamestate match_joined(player, 0, match_id);
    player_to_join->add_message_to_queue(match_joined);
}

void MatchManager::start_match(int player, int match_id)
{
    auto* requestor = all_players.get_by_id(player);
    auto* match = matches.get_by_id(match_id);
    if (match == nullptr) {
        Gamestate error(player, 1, "Match not found");
        requestor->add_message_to_queue(error);
        return;
    } else if (creators_by_match.at(match_id) != player) {
        Gamestate error(player, 1, "You cannot start this match");
        requestor->add_message_to_queue(error);
        return;
    }
    if (not match->is_connected()) {
        Gamestate match_started(player, 0, match_id);
        requestor->add_message_to_queue(match_started);
        match->send_start_message(player);
        match->start();
        creators_by_match.erase(match_id);
    } else {
        Gamestate error(player, 1, "Match already started");
        requestor->add_message_to_queue(error);
    }
}

void MatchManager::add_action_to_match(const Gameaction& action)
{
    auto* match = matches.select_one_if(
        [action](Match* m) {
            return m->is_player_in_match(action.player_id);
        }
    );
    match->add_action(action);
}

void MatchManager::send_matches_info(int player)
{
    std::cout << "el jugador " << player << " pidió info de los matches\n";
    std::list<Gamematch> matches_info;
    for (auto& [id, creator] : creators_by_match) {
        auto* match = matches.get_by_id(id);
        Gamematch match_info(id, creator, match->get_player_count());
        matches_info.push_back(match_info);
    }
    Gamestate matches_data(player, matches_info);
    Player* player_to_send = all_players.get_by_id(player);
    // print player pointer status or value
    std::cout << "before segfault??\n";
    std::cout << "Player to send: " << player_to_send << '\n';
    player_to_send->add_message_to_queue(matches_data);
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
                        create_match(action.player_id, action.is_multiplayer);
                        break;
                    case 5:
                        join_to_match(action.player_id, action.match);
                        break;
                    case 6:
                        start_match(action.player_id, action.match);
                        break;
                    case 7:
                        send_matches_info(action.player_id);
                        std::cout << "Sending matches info to player " << action.player_id << '\n';
                        break;
                    // case 8:
                    //     create_match(action.player_id, THREE_PLAYER_LIMIT);
                    //     break;
                    default:
                        add_action_to_match(action);
                        break;
                }
            // }
            // auto* match = matches.get_by_id(1);
            // for (int i = 1; i <= matches.size(); i++) {
            //     auto* match = matches.get_by_id(i);
            //     if (match != nullptr) {
            //         if (match->has_ended()) {
            //             // match->disconnect();
            //             // delete match;
            //             is_running.store(false);
            //         }
            //     }
            // }
            

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
        // for (int i = 1; i <= matches.size(); i++) {
        //     auto* match = matches.get_by_id(i);
        //     if (match != nullptr) {
        //         if (not match->is_connected()) {
        //             match->close_queue();
        //             delete match;
        //         }
        //         else {
        //             match->disconnect();
        //             delete match;
        //         }
        //     }
        // }
        matches.clear();
        is_running.store(false);
        _keep_running = false;
    } catch (const ClosedQueue& e) {
        is_running.store(false);
        _keep_running = false;

        matches.clear();
        std::cerr << "Se cerró la queue en el match manager!\n";

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

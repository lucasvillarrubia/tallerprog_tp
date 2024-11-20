#include "match.h"


Match::Match(int _id, int limit): id(_id), player_limit(limit), winner_id(-1), has_started(false), gameloop(player_list, user_commands) {}

void Match::start()
{
    try
    {
        std::cout << "comenzó la partida con " << player_list.size() << " jugadores\n";
        has_started = true;
        gameloop.start();
    } catch (const ClosedQueue& e) {
        std::cerr << "Se cerró la queue en la partida " << id << "!\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    } catch (...) {
        std::cerr << "Unknown exception on match " << id << ".\n";
    }
}

void Match::end()
{
    gameloop.stop();
    gameloop.join();
}

void Match::add_player(Player* player) { player_list.push_back(player); }

void Match::add_action(const Gameaction& action) { user_commands.push(action); }

bool Match::matches(int player_id) { return player_list.contains(player_id); }

bool Match::has_ended() { return (winner_id != -1 or player_list.size() == 0); }

bool Match::is_connected()
{ return has_started;
}

void Match::disconnect()
{
    user_commands.close();
    gameloop.stop();
    gameloop.join();
}

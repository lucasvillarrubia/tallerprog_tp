#include "match.h"


Match::Match(int _id): id(_id), winner_id(-1), has_started(false), gameloop(player_list, multiplayer_mode_by_player, user_commands) {}

void Match::start()
{
    try
    {
        // std::cout << "comenzó la partida con " << player_list.size() << " jugadores\n";
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

void Match::add_player(Player* player, int id, bool is_multiplayer) {
    player_list.push_back(player);
    multiplayer_mode_by_player.insert({id, is_multiplayer});
}

void Match::add_action(const Gameaction& action) { user_commands.push(action); }

bool Match::matches(int id) { return this->id == id; }

bool Match::is_player_in_match(int player_id)
{
    return player_list.contains(player_id);
}

bool Match::has_ended() { return (winner_id != -1 or player_list.size() == 0); }

bool Match::is_connected()
{ return has_started;
}

void Match::send_start_message(int creator)
{
    player_list.for_each([&](Player* player)
    {
        if (player->matches(creator)) return;
        Gamestate match_started(creator, 0, id);
        player->add_message_to_queue(match_started);
    });
}

void Match::disconnect()
{
    user_commands.close();
    gameloop.stop();
    gameloop.join();
}

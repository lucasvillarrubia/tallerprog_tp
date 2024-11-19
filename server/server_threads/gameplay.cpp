#include <iostream>

#include "gameplay.h"

#include "common/message.h"
#include "server/state_manager.h"


Gameplay::Gameplay(MonitoredList<Player*>& player_list, Queue<Gameaction>& usr_cmds):
        is_running(false), players(player_list), user_commands(usr_cmds) {
    // ducks_by_id.insert({1, Duck()});
    // ducks_by_id.insert({2, Duck()});
}

void Gameplay::broadcast_for_all_players(const Gamestate& state)
{
    players.for_each([&state](Player* player) {
        player->add_message_to_queue(state);
    });
}

void Gameplay::send_all_initial_coordinates()
{
    for (int i = 1; i <= players.size(); i++)
    {
        float x = 300.0f;
        float y = 300.0f;
        Duck duck;
        duck.set_position(x, y);
        duck.set_is_on_the_floor();
        ducks_by_id.insert({i, duck});
        Gamestate initial_duck_coordinates(i, x, y, 0, 0, 0, 1, 0.0f);
        broadcast_for_all_players(initial_duck_coordinates);
    }
    // for (auto& [id, duck]: ducks_by_id)
    // {
    //     if (id == 1)
    //     {
    //         float x = 125.0f;
    //         float y = 300.0f;
    //         Gamestate initial_duck_coordinates(id, x, y, 0, 0, 0, 1, 0.0f);
    //         duck.set_position(x, y);
    //         duck.set_is_on_the_floor();
    //         broadcast_for_all_players(initial_duck_coordinates);
    //     }
    //     else
    //     {
    //         Gamestate initial_duck_coordinates(id, 300.0f, 300.0f, 0, 0, 0, 1, 0.0f);
    //         duck.set_position(300.0f, 300.0f);
    //         broadcast_for_all_players(initial_duck_coordinates);
    //     }
    // }
}

void Gameplay::process_users_commands() {
    Gameaction command;
    while (user_commands.try_pop(command)) {
        StateManager::update_duck_state(ducks_by_id.at(command.player_id), command);
        Gamestate update = StateManager::get_duck_state(ducks_by_id.at(command.player_id), command.player_id);
        broadcast_for_all_players(update);
    }
}

void Gameplay::send_ducks_positions_updates(const unsigned int frame_delta)
{
    std::map<int, Coordinates> positions_by_id;
    for (auto& [id, duck]: ducks_by_id)
    {
        Coordinates before_coordinates = StateManager::get_duck_coordinates(duck);
        duck.update_position(frame_delta);
        Coordinates after_coordinates = StateManager::get_duck_coordinates(duck);
        terrain.adjust_position_for_collisions(duck, before_coordinates, after_coordinates);
        Coordinates updated_position = StateManager::get_duck_coordinates(duck);
        if ((id == 1) and ((updated_position.pos_Y != after_coordinates.pos_Y) or (updated_position.pos_X != after_coordinates.pos_X)))
        {
            std::cout << "x: " << updated_position.pos_X << " y: " << updated_position.pos_Y << "\n";
        }
        positions_by_id.insert({id, updated_position});
    }
    Gamestate update(positions_by_id);
    broadcast_for_all_players(update);
}

void Gameplay::run() {
    try
    {
        is_running.store(true);
        auto prev_time = std::chrono::steady_clock::now();
        send_all_initial_coordinates();
        while (is_running.load()) {
            auto current_time = std::chrono::steady_clock::now();
            auto frame_delta = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - prev_time).count();
            prev_time = current_time;
            process_users_commands();
            send_ducks_positions_updates(frame_delta);
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }
    catch (ClosedQueue const& e)
    {
        std::cerr << "Se cerró la queue del juego?! " << e.what() << '\n';
        is_running.store(false);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught in the gameplay thread: " << e.what() << '\n';
        is_running.store(false);
    }
    catch (...)
    {
        std::cerr << "Unknown exception on the gameloop.\n";
        is_running.store(false);
    }
}

void Gameplay::stop() {
    _keep_running = false;
    is_running.store(false);
}

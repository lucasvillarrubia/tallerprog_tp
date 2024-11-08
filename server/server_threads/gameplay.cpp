#include <iostream>

#include "common/message.h"

#include "gameplay.h"

#include "server//state_manager.h"


Gameplay::Gameplay(MonitoredList<Player*>& player_list, Queue<Gameaction>& usr_cmds):
        is_running(false), players(player_list), user_commands(usr_cmds) {
    ducks_by_id.insert({1, Duck()});
    ducks_by_id.insert({2, Duck()});
}

void Gameplay::process_users_commands() {
    Gameaction command;
    while (user_commands.try_pop(command)) {
        StateManager::update_duck_state(ducks_by_id.at(command.player_id), command);
        Gamestate update = StateManager::get_duck_state(ducks_by_id.at(command.player_id), command.player_id);
        // duck.update_position(frame_delta);
        // Gamestate update = StateManager::get_duck_state(duck);
        // // por ahora, primero me conecto con un solo pato
        // if (not duck.exited) players.broadcast(update);
    }
    // duck.update_position(frame_delta);
    // Gamestate update = StateManager::get_duck_state(duck);
    // // por ahora, primero me conecto con un solo pato
    // if (not duck.exited) players.broadcast(update);
}

void Gameplay::send_all_initial_coordinates()
{
    if ((players.size() == 1))
    {
        for (auto& [id, duck]: ducks_by_id)
        {
            if (id == 1)
            {
                Gamestate initial_duck_coordinates(id, 0.0f, 0.0f, 0, 0, 0, 1, 0.0f);
                players.broadcast(initial_duck_coordinates);
            }
            else
            {
                Gamestate initial_duck_coordinates(id, 590.0f, 0.0f, 0, 0, 0, 1, 0.0f);
                players.broadcast(initial_duck_coordinates);
            }
        }
        ya_entro_cliente = true;
    }

}

void Gameplay::send_ducks_positions_updates(const unsigned int frame_delta)
{
    std::map<int, Coordinates> positions_by_id;
    for (auto& [id, duck]: ducks_by_id)
    {
        duck.update_position(frame_delta);
        // if (terrain.is_duck_position_valid())
        positions_by_id.insert({id, StateManager::get_duck_coordinates(duck)});
    }
    Gamestate update(positions_by_id);
    players.broadcast(update);
}

void Gameplay::run() {
    try
    {
        ya_entro_cliente = false;
        is_running.store(true);
        auto prev_time = std::chrono::steady_clock::now();
        send_all_initial_coordinates();
        while (is_running.load()) {
            if (not ya_entro_cliente)
                send_all_initial_coordinates();
            auto current_time = std::chrono::steady_clock::now();
            process_users_commands();
            auto frame_delta = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - prev_time).count();
            prev_time = current_time;
            send_ducks_positions_updates(frame_delta);
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Maso 60 FPS
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

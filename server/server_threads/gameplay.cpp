#include <iostream>

#include "common/message.h"

#include "gameplay.h"

#include "server//state_manager.h"


Gameplay::Gameplay(MonitoredList& player_list, Queue<Gameaction>& usr_cmds):
        is_running(false), players(player_list), user_commands(usr_cmds) {
}

void Gameplay::process_users_commands() {
    Gameaction command;
    while (user_commands.try_pop(command)) {
        Gamestate update = StateManager::update_duck_state(duck, command);
        // por ahora, primero me conecto con un solo pato
        if (not duck.exited) players.broadcast(update);
    }
}

void Gameplay::run() {
    is_running.store(true);
    auto prev_time = std::chrono::steady_clock::now();
    while (is_running.load()) {
        try
        {
            auto current_time = std::chrono::steady_clock::now();
            auto frame_delta = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - prev_time).count();
            prev_time = current_time;
            duck.update_position(frame_delta);
            process_users_commands();
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Maso 60 FPS
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
}

void Gameplay::stop() {
    _keep_running = false;
    is_running.store(false);
}

#include <iostream>

#include "common/message.h"

#include "gameplay.h"


Gameplay::Gameplay(MonitoredList& player_list, Queue<Command>& usr_cmds):
        is_running(false), players(player_list), user_commands(usr_cmds) {
}

void Gameplay::process_users_commands() {
    Command command;
    while (user_commands.try_pop(command)) {
        int reward = 0;
        std::vector<char> name(command.player);
        Message box_picked_up(name, reward);
        box_picked_up.print();
        Command successful_pickup(name, reward);
        players.broadcast(successful_pickup);
    }
}

void Gameplay::run() {
    is_running.store(true);
    while (is_running.load()) {
        try
        {
            process_users_commands();
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

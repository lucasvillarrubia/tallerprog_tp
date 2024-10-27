#include <iostream>

#include "../common/message.h"

#include "gameplay.h"


const int DEFAULT_SLEEP_TIME = 200;


Gameplay::Gameplay(MonitoredList& player_list, Queue<Command>& usr_cmds):
        is_running(true), players(player_list), user_commands(usr_cmds) {
    boxes.insert({1, true});
    boxes.insert({2, true});
    boxes.insert({3, true});
    boxes.insert({4, true});
}

bool Gameplay::is_box_available(int id) {
    if (boxes.find(id) == boxes.end())
        return false;
    if (boxes.at(id)) {
        boxes[id] = false;
        iterations_left_by_dead_box.insert({id, default_respawn_iterations_by_box.at(id)});
        return true;
    }
    return false;
}

void Gameplay::process_users_commands() {
    Command command;
    while (user_commands.try_pop(command)) {
        if (is_box_available(command.id)) {
            int reward = rewards_by_box.at(command.id);
            std::vector<char> name(command.player);
            Message box_picked_up(name, reward);
            box_picked_up.print();
            Command successful_pickup(name, reward);
            players.broadcast(successful_pickup);
        }
    }
}

void Gameplay::check_for_boxes_respawns() {
    for (auto& [id, available]: boxes) {
        if (not available) {
            iterations_left_by_dead_box[id]--;
            if (iterations_left_by_dead_box.at(id) == 0) {
                boxes[id] = true;
                iterations_left_by_dead_box.erase(id);
                Message box_appeared;
                box_appeared.print();
                Command box_appearance;
                players.broadcast(box_appearance);
            }
        }
    }
}

void Gameplay::run() {
    while (is_running.load()) {
        try {
            process_users_commands();
            check_for_boxes_respawns();
            std::this_thread::sleep_for(std::chrono::milliseconds(DEFAULT_SLEEP_TIME));
        } catch (ClosedQueue const& e) {
            std::cerr << "Se cerró la queue del juego?! " << e.what() << '\n';
            is_running.store(false);
        } catch (const std::exception& e) {
            std::cerr << "Exception caught in the gameplay thread: " << e.what() << '\n';
            is_running.store(false);
        } catch (...) {
            std::cerr << "Unknown exception on the gameloop.\n";
            is_running.store(false);
        }
    }
}

void Gameplay::stop() {
    _keep_running = false;
    is_running.store(false);
}

Gameplay::~Gameplay() {}

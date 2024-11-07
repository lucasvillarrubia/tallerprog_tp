#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <atomic>

#include "common/hands_on_threads/queue.h"
#include "common/hands_on_threads/thread.h"
#include "server/command.h"
#include "server/duck.h"
#include "server/monitored_list.h"


class Gameplay: public Thread
{
private:
    std::atomic_bool is_running;
    MonitoredList<Player*>& players;
    Queue<Gameaction>& user_commands;
    Duck duck;
    void process_users_commands();
    void initialize_players();
    void send_all_initial_coordinates();
    void check_for_projectile_hit();
    void check_for_boxes_reappearances();
    void send_player_loss_update();
    void start_new_round();
    void send_victory_update();
public:
    Gameplay(MonitoredList<Player*>&, Queue<Gameaction>&);
    void run() override;
    void stop() override;
    ~Gameplay() override = default;
};


#endif
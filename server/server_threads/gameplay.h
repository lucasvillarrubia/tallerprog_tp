#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <atomic>
#include <map>

#include "common/hands_on_threads/queue.h"
#include "common/hands_on_threads/thread.h"
#include "server/command.h"
#include "server/terrain.h"
#include "server/duck.h"
#include "server/monitored_list.h"


const int DEFAULT_RESPAWN_ITERATIONS_BOX_1 = 15;
const int DEFAULT_RESPAWN_ITERATIONS_BOX_2 = 5;
const int DEFAULT_RESPAWN_ITERATIONS_BOX_3 = 20;
const int DEFAULT_RESPAWN_ITERATIONS_BOX_4 = 15;


class Gameplay: public Thread
{
private:
    std::atomic_bool is_running;
    MonitoredList<Player*>& players;
    const std::map<int, bool>& multiplayer_mode_by_player;
    Queue<Gameaction>& user_commands;
    std::map<int, Duck> ducks_by_id;
    // bool ya_entro_cliente;
    bool primera_caida;
    Terrain terrain;
    // Duck duck;

    std::map<int, int> rewards_by_box = {{0x01, 0x10}, {0x02, 0x11}, {0x03, 0x12}, {0x04, 0x13}};
    std::map<int, int> default_respawn_iterations_by_box = {
            {0x01, DEFAULT_RESPAWN_ITERATIONS_BOX_1},
            {0x02, DEFAULT_RESPAWN_ITERATIONS_BOX_2},
            {0x03, DEFAULT_RESPAWN_ITERATIONS_BOX_3},
            {0x04, DEFAULT_RESPAWN_ITERATIONS_BOX_4}};
    std::map<int, int> iterations_left_by_dead_box;
    std::map<int, bool> boxes;
    bool is_box_available(int);

    void process_users_commands();
    void initialize_players();
    void send_all_initial_coordinates();
    void send_ducks_positions_updates(unsigned int);
    void check_for_projectile_hit();
    void check_for_boxes_reappearances();
    void send_player_loss_update();
    void start_new_round();
    void send_victory_update();
    void broadcast_for_all_players(const Gamestate&);
public:
    Gameplay(MonitoredList<Player*>&, const std::map<int, bool>&, Queue<Gameaction>&);
    void run() override;
    void stop() override;
    ~Gameplay() override = default;
};


#endif
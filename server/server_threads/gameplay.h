#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <atomic>
#include <map>
#include <list>
#include <utility>

#include "common/hands_on_threads/queue.h"
#include "common/hands_on_threads/thread.h"
#include "server/terrain.h"
#include "server/duck.h"
#include "server/monitored_list.h"
#include "server/server_guns/gun.h"
#include "server/server_guns/ammo.h"
#include "server/server_guns/AK47.h"
#include "server/server_guns/Banana.h"
#include "server/server_guns/DuelPistol.h"
#include "server/server_guns/CowboyPistol.h"
#include "server/server_guns/Magnum.h"
#include "server/server_guns/Sniper.h"
#include "server/server_guns/SpawnPlace.h"
#include "server/player.h"



const int DEFAULT_RESPAWN_ITERATIONS_BOX_1 = 15;
const int DEFAULT_RESPAWN_ITERATIONS_BOX_2 = 5;
const int DEFAULT_RESPAWN_ITERATIONS_BOX_3 = 20;
const int DEFAULT_RESPAWN_ITERATIONS_BOX_4 = 15;


#include "common/drawingdata.h"


class Gameplay: public Thread
{
private:
    std::atomic_bool is_running;
    MonitoredList<Player*>& players;
    std::list<int> disconnected_players;
    const std::map<int, bool>& multiplayer_mode_by_player;
    Queue<Gameaction>& user_commands;
    std::map<int, Duck> ducks_by_id;
    std::map<int, Gun*> guns_by_id;
    std::list<std::pair<int, Ammo*>> bullets_by_id;
    std::list<SpawnPlace> spawn_places;
    int guns_in_map;
    int bullets_fired;
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
    void try_to_grab(Duck& duck);
    void try_to_shoot(Duck& duck);
    void try_to_slip(Duck& duck);
    void send_guns_positions_updates(unsigned int);
    void send_bullets_positions_updates(unsigned int);
    void update_spawn_places();
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

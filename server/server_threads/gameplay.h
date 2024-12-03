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
#include "server/server_guns/Grenade.h"
#include "server/server_guns/Banana.h"
#include "server/server_guns/CowboyPistol.h"
#include "server/server_guns/DuelPistol.h"
#include "server/server_guns/Grenade.h"
#include "server/server_guns/Magnum.h"
#include "server/server_guns/Sniper.h"
#include "server/server_guns/SpawnPlace.h"
#include "server/server_guns/PewPewLaser.h"
#include "server/server_guns/LaserRifle.h"
#include "server/player.h"



const int DEFAULT_RESPAWN_ITERATIONS_BOX_1 = 15;
const int DEFAULT_RESPAWN_ITERATIONS_BOX_2 = 5;
const int DEFAULT_RESPAWN_ITERATIONS_BOX_3 = 20;
const int DEFAULT_RESPAWN_ITERATIONS_BOX_4 = 15;


#include "common/drawingdata.h"
#include "common/color.h"


class Gameplay: public Thread
{
private:
    std::atomic_bool is_running;
    MonitoredList<Player*>& players;
    std::list<int> disconnected_players;
    std::map<int, bool>& multiplayer_mode_by_player;
    Queue<Gameaction>& user_commands;
    int& winner_id;
    std::map<int, Duck> ducks_by_id;
    std::map<int, Color> duck_colors_by_id;
    std::map<int, Gun*> guns_by_id;
    std::list<std::pair<int, Ammo*>> bullets_by_id;
    std::map<int, int> scores_by_id;
    std::list<SpawnPlace> spawn_places;
    int guns_in_map;
    int bullets_fired;
    bool primera_caida;
    Terrain terrain;
    int current_round;
    bool round_is_over;
    bool test_mode;
    void process_users_commands();
    void initialize_players();
    void send_all_initial_coordinates();
    void send_ducks_positions_updates(unsigned int);
    void try_to_grab(Duck& duck);
    void try_to_shoot(Duck& duck);
    void try_to_slip_or_explode(Duck& duck, int duck_id);
    void send_guns_positions_updates(unsigned int);
    void send_bullets_positions_updates(unsigned int);
    void update_spawn_places();
    void check_for_winner();
    void broadcast_for_all_players(const Gamestate&);
public:
    Gameplay(MonitoredList<Player*>&, std::map<int, bool>&, Queue<Gameaction>&, int&);
    void run() override;
    bool is_game_on() { return is_running.load(); }
    void stop() override;
    ~Gameplay() override = default;
};


#endif

#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <atomic>
#include <map>
#include <list>
#include <utility>

#include "common/hands_on_threads/queue.h"
#include "common/hands_on_threads/thread.h"
#include "server/command.h"
#include "server/terrain.h"
#include "server/duck.h"
#include "server/monitored_list.h"
#include "server/server_guns/gun.h"
#include "server/server_guns/ammo.h"
#include "server/server_guns/AK47.h"
#include "server/server_guns/DuelPistol.h"
#include "server/server_guns/CowboyPistol.h"
#include "server/server_guns/Magnum.h"


#include "common/drawingdata.h"

class Gameplay: public Thread
{
private:
    std::atomic_bool is_running;
    MonitoredList<Player*>& players;
    Queue<Gameaction>& user_commands;
    std::map<int, Duck> ducks_by_id;
    std::map<int, Gun*> guns_by_id;
    std::list<std::pair<int, Ammo*>> bullets_by_id;
    int bullets_fired;
    // bool ya_entro_cliente;
    bool primera_caida;
    Terrain terrain;
    // Duck duck;
    void process_users_commands();
    void initialize_players();
    void send_all_initial_coordinates();
    void send_ducks_positions_updates(unsigned int);
    void try_to_grab(Duck& duck);
    void try_to_shoot(Duck& duck);
    void send_guns_positions_updates();
    void send_bullets_positions_updates(unsigned int);
    void check_for_projectile_hit();
    void check_for_boxes_reappearances();
    void send_player_loss_update();
    void start_new_round();
    void send_victory_update();
    void broadcast_for_all_players(const Gamestate&);
public:
    Gameplay(MonitoredList<Player*>&, Queue<Gameaction>&);
    void run() override;
    void stop() override;
    ~Gameplay() override = default;
};


#endif

#ifndef CLIENT_STATE_MANAGER_H
#define CLIENT_STATE_MANAGER_H


#include <list>
#include <mutex>

#include "character.h"
#include "common/gamedata.h"
#include "gun.h"
#include "bullet.h"
#include "explosion.h"


class StateManager
{
private:
    std::mutex mtx;
    std::list<Character> dukis;
    std::list<Gun> guns;
    std::list<Bullet> bullets;
    std::list<Explosion> explosions;
    int round;
    void update_duck_position(Character&, const Coordinates&, const float);
    void update_gun_position(Gun&, const Coordinates&);
    void update_bullet_position(Bullet&, const Coordinates&);
    void update_duck_state(const Gamestate& update);
    void explode_grenade(const int id);
public:
    StateManager();
    void update(const Gamestate&);
    void update_ducks(const Gamestate&);
    void update_guns(const Gamestate&);
    void update_bullets(const Gamestate&);
    void destroy_bullet(const int id);
    void clear_destroyed_gun(int id);
    void set_explosion_phase(const unsigned int);
    void reset();
    // bool is_moving_to_the_right();
    // Coordinates get_coordinates();
    // int get_movement_phase(unsigned int);
    std::list<Character> get_characters_data();
    std::list<Gun> get_guns_data();
    std::list<Bullet> get_bullets_data();
    std::list<Explosion> get_explosions_data();
    ~StateManager() = default;
};



#endif

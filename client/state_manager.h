#ifndef CLIENT_STATE_MANAGER_H
#define CLIENT_STATE_MANAGER_H


#include <list>
#include <mutex>

#include "character.h"
#include "gun.h"
#include "common/gamedata.h"


class StateManager
{
private:
    std::mutex mtx;
    std::list<Character> dukis;
    std::list<Gun> guns;
    void update_duck_position(Character&, const Coordinates&);
    void update_gun_position(Gun&, const Coordinates&);
    void update_duck_state(const Gamestate& update);
public:
    StateManager();
    void update(const Gamestate&);
    void update_ducks(const Gamestate&);
    void update_guns(const Gamestate&);
    // bool is_moving_to_the_right();
    // Coordinates get_coordinates();
    // int get_movement_phase(unsigned int);
    std::list<Character> get_characters_data();
    std::list<Gun> get_guns_data();
    ~StateManager() = default;
};



#endif

#ifndef CLIENT_STATE_MANAGER_H
#define CLIENT_STATE_MANAGER_H


#include <mutex>

#include "character.h"
#include "common/gamedata.h"


class StateManager
{
private:
    std::mutex mtx;
    Character duki;
public:
    StateManager();
    void update_duck(const Gamestate&);
    bool is_moving_to_the_right();
    Coordinates get_coordinates();
    int get_movement_phase(unsigned int);
    ~StateManager() = default;
};



#endif

#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H


#include "duck.h"

#include "common/gamedata.h"


class StateManager
{
public:
    static void update_duck_state(Duck&, const Gameaction&);
    static Gamestate get_duck_state(Duck&, int);
    static Coordinates get_duck_coordinates(Duck&);
    static float get_duck_speed(Duck& duck);
    static int get_duck_is_alive(Duck& duck);
};


#endif

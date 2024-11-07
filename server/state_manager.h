#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H


#include "duck.h"

#include "common/gamedata.h"


class StateManager
{
public:
    // Por ahora esto es static porque la clase no tiene atributos, falta completarla.
    static void update_duck_state(Duck&, const Gameaction&);
    static Gamestate get_duck_state(Duck&);

};


#endif

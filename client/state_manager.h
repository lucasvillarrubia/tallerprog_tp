#ifndef CLIENT_STATE_MANAGER_H
#define CLIENT_STATE_MANAGER_H


#include <SDL_events.h>

#include "character.h"
#include "common/gamedata.h"


class StateManager
{
public:
    // Por ahora esto es static porque la clase no tiene atributos, falta completarla.
    static void update_duck_state(Character&, SDL_Event);
    static void update_duck(Character&, Gamestate&);
};



#endif

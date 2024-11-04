#include "state_manager.h"

#include "common/gamedata.h"


const float JUMP_STRENGTH = 15.0f;
const float FLAP_STRENGTH = 1.0f; // Fuerza de aleteo en caída

void StateManager::update_duck_state(Character& duck, const SDL_Event event)
{
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_RIGHT:
            duck.is_running = true;
            duck.moving_right = true;
            break;
        case SDLK_LEFT:
            duck.is_running = true;
            duck.moving_right = false;
            break;
        case SDLK_SPACE:
            if (!duck.is_jumping) { // Si no está en el aire, inicia el salto
                duck.is_jumping = true;
                duck.is_flapping = true;
                duck.jump_velocity = JUMP_STRENGTH;
            } else if (!duck.is_flapping && duck.jump_velocity < 0) { // Si está en caída, activa el aleteo
                duck.is_flapping = true;
                duck.jump_velocity = FLAP_STRENGTH; // Aplica fuerza adicional en el aire
            }
            break;
        }
    } else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
        case SDLK_RIGHT:
        case SDLK_LEFT:
            duck.is_running = false;
            break;
        case SDLK_SPACE:
            duck.is_flapping = false;
            break;
        }
    }
}

void StateManager::update_duck(Character& duki, Gamestate& new_state)
{
    duki.pos_X = new_state.pos_X;
    duki.pos_Y = new_state.pos_Y;
    duki.is_running = new_state.is_running;
    duki.is_jumping = new_state.is_jumping;
    duki.is_flapping = new_state.is_flapping;
    duki.moving_right = new_state.move_direction;
    duki.jump_velocity = new_state.jump_speed;
}

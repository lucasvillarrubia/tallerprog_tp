#include "state_manager.h"


const float JUMP_STRENGTH = 15.0f;


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
            if (!duck.is_jumping)
            {
                duck.is_jumping = true;
                duck.is_flapping = true;
                duck.jump_velocity = JUMP_STRENGTH;
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
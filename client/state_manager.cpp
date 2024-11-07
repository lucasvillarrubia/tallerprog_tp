#include "state_manager.h"
#include "item_box.h"

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
                if (!duck.is_jumping) {
                    duck.is_jumping = true;
                    duck.is_flapping = true;
                    duck.jump_velocity = JUMP_STRENGTH;
                } else if (!duck.is_flapping && duck.jump_velocity < 0) {
                    duck.is_flapping = true;
                    duck.jump_velocity = FLAP_STRENGTH;
                }
                break;
            case SDLK_DOWN:
                duck.is_ducking = true;
                break;
            case SDLK_g:
                duck.is_grabbing = true;
                break;
            case SDLK_f:
            	duck.is_shooting = true;
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
            case SDLK_DOWN:
                duck.is_ducking = false;
                break;
            case SDLK_g:
                duck.is_grabbing = false;
                break;
            case SDLK_f:
            	duck.is_shooting = false;
            	break;
        }
    }
}

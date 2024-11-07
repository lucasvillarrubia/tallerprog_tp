#include "state_manager.h"


const int QUIT = 1;
const int KEYDOWN = 2;
const int KEYUP = 3;

const int ESCAPE = 9;
const int RIGHT = 1;
const int LEFT = 2;
const int SPACE = 3;

const float JUMP_STRENGTH = 15.0f;
const float FLAP_STRENGTH = 1.0f;


void StateManager::update_duck_state(Duck& duck, const Gameaction& event)
{
    if (event.type == QUIT)
    {
        duck.exited = true;
        return;
    }
    if (event.type == KEYDOWN) {
        switch (event.key) {
        case ESCAPE:
            duck.exited = true;
            return;
        case RIGHT:
            duck.is_running = true;
            duck.moving_right = true;
            break;
        case LEFT:
            duck.is_running = true;
            duck.moving_right = false;
            break;
        case SPACE:
            if (!duck.is_jumping) {
                duck.is_jumping = true;
                duck.is_flapping = true;
                duck.jump_velocity = JUMP_STRENGTH;
            } else if (!duck.is_flapping && duck.jump_velocity < 0) {
                duck.is_flapping = true;
                duck.jump_velocity = FLAP_STRENGTH;
            }
            break;
        }
    } else if (event.type == KEYUP) {
        switch (event.key) {
        case RIGHT:
        case LEFT:
            duck.is_running = false;
            break;
        case SPACE:
            duck.is_flapping = false;
            break;
        }
    }
}

Gamestate StateManager::get_duck_state(Duck& duck)
{
    return {
        1, // sería el player id
        duck.pos_X,
        duck.pos_Y,
        duck.is_running ? 1 : 0,
        duck.is_jumping ? 1 : 0,
        duck.is_flapping ? 1 : 0,
        duck.moving_right ? 1 : 0,
        duck.jump_velocity
    };
}

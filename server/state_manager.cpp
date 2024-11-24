#include "state_manager.h"


const int QUIT = 1;
const int KEYDOWN = 2;
const int KEYUP = 3;

const int ESCAPE = 9;
const int RIGHT = 1;
const int LEFT = 2;
const int SPACE = 3;
const int g = 4;
const int f = 5;

const float JUMP_STRENGTH = 15.0f;
const float FLAP_STRENGTH = 1.0f;


void StateManager::update_duck_state(Duck& duck, const Gameaction& event)
{
    if (event.type == QUIT)
    {
        // duck.exited = true;
        return;
    }
    if (event.type == KEYDOWN) {
        switch (event.key) {
        case ESCAPE:
            // duck.exited = true;
            return;
        case RIGHT:
            if (not duck.is_stomping_on_wall_from_right) {
                duck.is_running = true;
                duck.moving_right = true;
            }
            if (duck.is_stomping_on_wall_from_right) {
                duck.is_stomping_on_wall_from_right = false;
            }
            break;
        case LEFT:
            if (not duck.is_stomping_on_wall_from_left) {
                duck.is_running = true;
                duck.moving_right = false;
            }
            if (duck.is_stomping_on_wall_from_left) {
                duck.is_stomping_on_wall_from_left = false;
            }
            break;
        case SPACE:
            if (!duck.is_jumping)
            {
                duck.is_jumping = true;
                duck.is_on_the_floor = false;          
                duck.is_flapping = true;
                duck.jump_velocity = JUMP_STRENGTH;
            }
             else if (duck.is_jumping && duck.jump_velocity < 0) {
                 duck.is_flapping = true;
                 duck.jump_velocity = FLAP_STRENGTH;
             }
             if (!duck.is_flapping && duck.jump_velocity < 0) {
                 duck.is_flapping = true;
                 duck.jump_velocity = FLAP_STRENGTH;
             }
            break;
        case g:
        	duck.is_grabbing = true;
        	break;
        case f:
        	duck.is_shooting = true;
        	break;
        }
    } else if (event.type == KEYUP) {
        switch (event.key) {
        case RIGHT:
        case LEFT:
            duck.is_running = false;
            break;
        // case SPACE:
        //     duck.is_flapping = false;
        //     break;
        case g:
        	duck.is_grabbing = false;
        	break;
        case f:
        	duck.is_shooting = false;
        	break;
        }
    }
}

Gamestate StateManager::get_duck_state(Duck& duck, int id)
{
    return {
        id,
        duck.is_running ? 1 : 0,
        duck.is_jumping ? 1 : 0,
        duck.is_flapping ? 1 : 0,
        duck.moving_right ? 1 : 0,
        duck.is_alive ? 1 : 0
    };
}

Coordinates StateManager::get_duck_coordinates(Duck& duck)
{
    return { duck.pos_X, duck.pos_Y };
}

float StateManager::get_duck_speed(Duck& duck)
{
    return duck.jump_velocity;
}

int StateManager::get_duck_is_alive(Duck& duck)
{
    return duck.is_alive;
}

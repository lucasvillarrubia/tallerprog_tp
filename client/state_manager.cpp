#include "state_manager.h"

#include "common/gamedata.h"


const float JUMP_STRENGTH = 15.0f;
const float FLAP_STRENGTH = 1.0f;
const int AVAILABLE_MOVEMENT_SPRITES = 6;


StateManager::StateManager() {}

void StateManager::update_duck(const Gamestate& new_state) {
    std::unique_lock<std::mutex> lck(mtx);
    duki.pos_X = new_state.pos_X;
    duki.pos_Y = new_state.pos_Y;
    duki.is_running = new_state.is_running;
    duki.is_jumping = new_state.is_jumping;
    duki.is_flapping = new_state.is_flapping;
    duki.moving_right = new_state.move_direction;
    duki.jump_velocity = new_state.jump_speed;
}

bool StateManager::is_moving_to_the_right()
{
    std::unique_lock<std::mutex> lck(mtx);
    return duki.moving_right;
}

Coordinates StateManager::get_coordinates()
{
    std::unique_lock<std::mutex> lck(mtx);
    return {duki.pos_X, duki.pos_Y};
}

int StateManager::get_movement_phase (const unsigned int frame_ticks)
{
    std::unique_lock<std::mutex> lck(mtx);
    return (duki.is_running ? ((frame_ticks / 100) % AVAILABLE_MOVEMENT_SPRITES) : 0);
}

#include "state_manager.h"

#include <iostream>
#include <utility>

#include "common/gamedata.h"


const float JUMP_STRENGTH = 15.0f;
const float FLAP_STRENGTH = 1.0f;
// const int AVAILABLE_MOVEMENT_SPRITES = 6;


StateManager::StateManager() {}

void StateManager::update(const Gamestate& update)
{
    std::unique_lock<std::mutex> lck(mtx);
    // std::cout << "el tipo de update que llegó es: " << update.type << "\n";
    switch (update.type)
    {
    case 1:
        {
            Character new_duki(update.player_id);
            new_duki.pos_X = update.pos_X;
            new_duki.pos_Y = update.pos_Y;
            dukis.push_back(new_duki);
            break;
        }
    case 3:
        update_duck_state(update);
        break;
    case 4:
    	update_guns(update);
    	break;
    case 5:
    	{
    	Gun new_gun(update.gun_id, update.type_gun, update.pos_X, update.pos_Y);
    	guns.push_back(new_gun);
    	break;
    	}
    default:
        update_ducks(update);
        break;
    }

}

void StateManager::update_ducks(const Gamestate& update)
{
    for (auto& duki : dukis)
    {
        if (update.positions_by_id.contains(duki.id))
            update_duck_position(duki, update.positions_by_id.at(duki.id));
    }
}

void StateManager::update_guns(const Gamestate& update)
{
    for (auto& gun : guns)
    {
        if (update.guns_positions_by_type.contains(gun.id))
            update_gun_position(gun, update.guns_positions_by_type.at(gun.id).second);
    }
}

void StateManager::update_duck_position(Character& duki, const Coordinates& new_position) {
    duki.pos_X = new_position.pos_X;
    duki.pos_Y = new_position.pos_Y;
}

void StateManager::update_gun_position(Gun& gun, const Coordinates& new_position) {
    gun.pos_X = new_position.pos_X;
    gun.pos_Y = new_position.pos_Y;
}

void StateManager::update_duck_state(const Gamestate& update)
{
    for (auto& duki : dukis)
    {
        if (duki.id == update.player_id)
        {
            duki.is_running = update.is_running;
            duki.is_jumping = update.is_jumping;
            duki.is_flapping = update.is_flapping;
            duki.is_grabbing = update.is_grabbing;
            duki.is_shooting = update.is_shooting;
            duki.with_gun = update.with_gun;
    		duki.with_armor = update.with_armor;
    		duki.with_helmet = update.with_helmet;
            duki.moving_right = update.move_direction;
            // std::cout << "se está moviendo a la derecha?: " << (duki.moving_right ? "SI\n" : "NO\n");
            break;
        }
    }
}

// bool StateManager::is_moving_to_the_right()
// {
//     std::unique_lock<std::mutex> lck(mtx);
//     return duki.moving_right;
// }
//
// Coordinates StateManager::get_coordinates()
// {
//     std::unique_lock<std::mutex> lck(mtx);
//     return {duki.pos_X, duki.pos_Y};
// }
//
// int StateManager::get_movement_phase (const unsigned int frame_ticks)
// {
//     std::unique_lock<std::mutex> lck(mtx);
//     return (duki.is_running ? ((frame_ticks / 100) % AVAILABLE_MOVEMENT_SPRITES) : 0);
// }

std::list<Character> StateManager::get_characters_data()
{
    return dukis;
}

std::list<Gun> StateManager::get_guns_data() {
	return guns;
}

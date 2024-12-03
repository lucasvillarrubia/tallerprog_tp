#include "state_manager.h"

#include <iostream>

#include "common/gamedata.h"
#include "texture_manager.h"

const float JUMP_STRENGTH = 15.0f;
const float FLAP_STRENGTH = 1.0f;
const int DUCK_IS_ALIVE = 1;
// const int AVAILABLE_MOVEMENT_SPRITES = 6;


StateManager::StateManager(const int& id, int& winner, const bool& mode): current_id(id), match_winner(winner), is_multiplayer(mode), round(1), alive(true), match_ended(false), shots(0) {}

void StateManager::update(const Gamestate& update)
{
    // std::unique_lock<std::mutex> lck(mtx);
    switch (update.type)
    {
    case 1:
        {
            if (update.is_alive == DUCK_IS_ALIVE) {
                Character new_duki(update.player_id);
                new_duki.pos_X = update.pos_X;
                new_duki.pos_Y = update.pos_Y;
                new_duki.color = update.color;
                dukis.push_back(new_duki);
                scores_by_id.insert({update.player_id, 0});
            }
            break;
        }
    case 3:
        update_duck_state(update);
        break;
    case 5:
    	{
    		Gun new_gun(update.object_id);
    		new_gun.pos_X = update.pos_X;
    		new_gun.pos_Y = update.pos_Y;
    		new_gun.type = update.type_gun;
    		new_gun.pointing_to_the_right = update.move_direction;
    		guns.push_back(new_gun);
    		break;
    	}
    case 6:
    	update_guns(update);
    	break;
    case 7:
    	{
    		Bullet new_bullet(update.object_id);
    		new_bullet.origin_X = update.pos_X;
    		new_bullet.origin_Y = update.pos_Y;
    		new_bullet.pos_X = update.pos_X;
    		new_bullet.pos_Y = update.pos_Y;
    		new_bullet.type = update.type_gun;
    		new_bullet.moving_right = update.move_direction;
    		new_bullet.moving_up = update.is_pointing_upwards;
    		bullets.push_back(new_bullet);
    		shots++;
    		break;
    	}
    case 8:
    	update_bullets(update);
    	break;
    case 9:
    	destroy_bullet(update.object_id);
    	break;
    case 15:
    	explode_grenade(update.object_id);
    	break;
    case 13:
        reset();
        if (update.round == 0) {
            match_ended = true;
            match_winner = update.player_id;
        } else {
            round = update.round;
            scores_by_id.at(update.player_id) += 1;
            // std::cout << "Round: " << round << std::endl;
        }
        break;
    default:
        update_ducks(update);
        break;
    }
    dukis.remove_if([this](const Character& duki) {
        if (not duki.is_alive) {
            if ((current_id == duki.id) and not is_multiplayer) {
                alive = false;
            }
            return true;
        } else {
            return false;
        }
    });
}

void StateManager::update_ducks(const Gamestate& update)
{
    for (auto& duki : dukis)
    {
        if (update.positions_by_id.contains(duki.id))
            update_duck_position(duki, update.positions_by_id.at(duki.id), update.speeds_by_id.at(duki.id));
    }
}

void StateManager::update_guns(const Gamestate& update)
{
    for (auto& gun : guns)
    {
        if (update.guns_positions_by_id.contains(gun.id)) {
        	gun.pointing_to_the_right = update.guns_positions_by_id.at(gun.id).first.right;
        	gun.shooting = update.guns_positions_by_id.at(gun.id).first.shooting;
        	gun.pointing_up = update.guns_positions_by_id.at(gun.id).first.up;
            update_gun_position(gun, update.guns_positions_by_id.at(gun.id).second);
        }
    }
}

void StateManager::update_bullets(const Gamestate& update)
{
    for (auto& bullet : bullets)
    {
        if (update.bullets_positions_by_id.contains(bullet.id)){
            update_bullet_position(bullet, update.bullets_positions_by_id.at(bullet.id));
        }
    }
}

void StateManager::update_duck_position(Character& duki, const Coordinates& new_position, const float speed) {
    duki.pos_X = new_position.pos_X;
    duki.pos_Y = new_position.pos_Y;
    duki.jump_velocity = speed;
}

void StateManager::update_gun_position(Gun& gun, const Coordinates& new_position) {
    gun.pos_X = new_position.pos_X;
    gun.pos_Y = new_position.pos_Y;
}

void StateManager::update_bullet_position(Bullet& bullet, const Coordinates& new_position) {
    bullet.pos_X = new_position.pos_X;
    bullet.pos_Y = new_position.pos_Y;
}

void StateManager::destroy_bullet(const int id) {
	for (auto& bullet : bullets) {
		if (bullet.id == id) {
			bullet.destroyed = true;
		}
	}
	bullets.remove_if([](auto& bullet){ return bullet.destroyed; });
}

void StateManager::explode_grenade(const int id) {
	for (auto& gun : guns) {
		if (gun.id == id) {
			Explosion explosion(gun.pos_X, gun.pos_Y);
			explosions.push_back(explosion);
			gun.is_destroyed = true;
		}
	}
	// std::cout<<"se recibió la explosion"<<std::endl;
	clear_destroyed_gun(id);
}

void StateManager::reset()
{
    // std::unique_lock<std::mutex> lck(mtx);
    alive = true;
    dukis.clear();
    guns.clear();
    bullets.clear();
}

void StateManager::set_new_game()
{
    match_ended = false;
    round = 1;
    alive = true;
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
            duki.moving_right = update.move_direction;
            duki.is_alive = update.is_alive;
            duki.is_slipping = update.is_slipping;
            duki.is_pointing_upwards = update.is_pointing_upwards;
            duki.is_ducking = update.is_ducking;
            break;
        }
    }
}

void StateManager::clear_destroyed_gun(int id) {
	// std::cout<<"size antes del remove: "<<guns.size()<<std::endl;
	guns.remove_if([&id](auto& gun){ return gun.id == id; });
	// std::cout<<"size despues del remove: "<<guns.size()<<std::endl;
}

void StateManager::set_explosion_phase(const unsigned int frame) {
	for (auto& explosion : explosions) {
		if (!explosion.end)
			explosion.set_next_phase(frame);
	}
	explosions.remove_if([](auto& explosion){ return explosion.end; });
}

std::list<Character> StateManager::get_characters_data()
{
    // std::unique_lock<std::mutex> lck(mtx);
    return dukis;
}

std::list<Gun> StateManager::get_guns_data() {
    // std::unique_lock<std::mutex> lck(mtx);
	return guns;
}

std::list<Bullet> StateManager::get_bullets_data() {
    // std::unique_lock<std::mutex> lck(mtx);
	return bullets;
}

State StateManager::get_state()
{
    // std::unique_lock<std::mutex> lck(mtx);
    return {dukis, guns, bullets};
}

std::list<Explosion> StateManager::get_explosions_data() {
	return explosions;

}

std::map<int, int> StateManager::get_scores() {
    return scores_by_id;
}

bool StateManager::reproducir_disparo() {
	if (shots > 0) {
		shots--;
		return true;
	}
	return false;
}

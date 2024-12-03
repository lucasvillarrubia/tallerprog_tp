#include <iostream>

#include "gameplay.h"
#include "server/state_manager.h"



const int MULTIPLAYER_ID_OFFSET = 128;
const int SCORES_TO_WIN = 5;


Gameplay::Gameplay(MonitoredList<Player*>& player_list, std::map<int, bool>& multiplayer_modes, Queue<Gameaction>& usr_cmds, int& winner):

        is_running(false),
        players(player_list),
        multiplayer_mode_by_player(multiplayer_modes),
        user_commands(usr_cmds),
        winner_id(winner),
        terrain(spawn_places, guns_by_id, ducks_by_id),
        current_round(1),
        test_mode(false)
{}


void Gameplay::broadcast_for_all_players(const Gamestate& state)
{
    players.for_each([&](Player* player) {
        for (auto& id : disconnected_players)
        {
            if (player->matches(id))
                return;
        }
        player->add_message_to_queue(state);
    }
    );
}

void Gameplay::send_all_initial_coordinates()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (auto& [id, duck]: ducks_by_id)
    {
        Coordinates position = StateManager::get_duck_coordinates(duck);
        Color color(rand() % 255, rand() % 255, rand() % 255);
        if (duck_colors_by_id.find(id) == duck_colors_by_id.end())
            duck_colors_by_id.insert({id, color});
        Gamestate initial_duck_coordinates(
            id,
            position.pos_X,
            position.pos_Y,
            0,
            0,
            0,
            1,
            StateManager::get_duck_is_alive(duck),
            0.0f,
            color
        );
        broadcast_for_all_players(initial_duck_coordinates);
    }
    guns_in_map = guns_by_id.size();
    for (auto& [id, gun] : guns_by_id) {
    	Coordinates position = gun->getPosition();
    	Gamestate initial_gun_coordinates(
    		id,
			position.pos_X,
            position.pos_Y,
            gun->getType(),
            gun->is_pointing_to_the_right() ? 1 : 0
    	);
    	broadcast_for_all_players(initial_gun_coordinates);
    }
    terrain.set_spawn_places();
}

void Gameplay::process_users_commands() {
    Gameaction command;
    while (user_commands.try_pop(command)) {
        if (command.type == 9 || command.key == 9) {
            disconnected_players.push_back(command.player_id);
            ducks_by_id.at(command.player_id).set_is_NOT_alive();
            Gamestate duck_is_dead(StateManager::get_duck_state(ducks_by_id.at(command.player_id), command.player_id));
            broadcast_for_all_players(duck_is_dead);
            ducks_by_id.erase(command.player_id);
            multiplayer_mode_by_player.erase(command.player_id);
            players.remove_if([&](Player* player) { return player->matches(command.player_id); });
            continue;
        }
        if (command.is_multiplayer) {
            command.player_id += MULTIPLAYER_ID_OFFSET;
        }
        StateManager::update_duck_state(ducks_by_id.at(command.player_id), command);
        Gamestate update = StateManager::get_duck_state(ducks_by_id.at(command.player_id), command.player_id);
        broadcast_for_all_players(update);
    }
}

void Gameplay::initialize_players()
{
    if ((players.size() == 1) and not multiplayer_mode_by_player.begin()->second) {
        test_mode = true;   
    }
    for (auto& [id, is_multiplayer] : multiplayer_mode_by_player)
    {
        Duck duck;
        ducks_by_id.insert({id, duck});
        scores_by_id.insert({id, 0});
        if (test_mode) {
            for (int i = 2; i < 5; i++) {
                if (i != id) {
                    Duck test_duck;
                    ducks_by_id.insert({i, test_duck});
                }
            }
            break;
        }
        if (is_multiplayer) {
            Duck second;
            ducks_by_id.insert({id + MULTIPLAYER_ID_OFFSET, second});
            scores_by_id.insert({id + MULTIPLAYER_ID_OFFSET, 0});
        }
    }
    terrain.set_ducks_positions();
}

void Gameplay::send_ducks_positions_updates(const unsigned int frame_delta)
{
    std::map<int, Coordinates> positions_by_id;
    std::map<int, float> speeds_by_id;
    for (auto& [id, duck]: ducks_by_id)
    {
        if (StateManager::get_duck_is_alive(duck) == 0)
        {
            continue;
        }
        Coordinates before_coordinates = StateManager::get_duck_coordinates(duck);
        duck.update_position(frame_delta);
        Coordinates after_coordinates = StateManager::get_duck_coordinates(duck);
        terrain.adjust_position_for_collisions(duck, before_coordinates, after_coordinates);
        Coordinates updated_position = StateManager::get_duck_coordinates(duck);
        try_to_grab(duck);
        try_to_shoot(duck);
        try_to_slip_or_explode(duck, id);
        positions_by_id.insert({id, updated_position});
        speeds_by_id.insert({id, StateManager::get_duck_speed(duck)});
        if (StateManager::get_duck_is_alive(duck) == 0)
        {
            Gamestate duck_is_dead(StateManager::get_duck_state(duck, id));
            broadcast_for_all_players(duck_is_dead);
        }
    }
    Gamestate update(positions_by_id, speeds_by_id);
    broadcast_for_all_players(update);
}

void Gameplay::try_to_grab(Duck& duck) {
	Coordinates after_coordinates = StateManager::get_duck_coordinates(duck);
	if (duck.wants_to_grab()) {
		if (duck.have_a_gun()) {
        	guns_by_id.at(duck.get_gun_id())->dropped();
        	duck.drop_gun();
        	duck.stop_grab();
        } else {
        	for (auto& spawn_place : spawn_places) {
        		if (spawn_place.is_duck_position_valid(after_coordinates.pos_X, after_coordinates.pos_Y)) {
        			if (spawn_place.is_gun_spawned()) {
        				spawn_place.pick_item();
        			}
        		}
        	}
        	for (auto& [id,gun] : guns_by_id) {
        		if (gun->is_duck_position_valid(after_coordinates.pos_X, after_coordinates.pos_Y)) {
        			duck.pickup_gun(id);
        			gun->collected();
        			duck.stop_grab();
        		}
        	}
       	}
	}
}

void Gameplay::try_to_shoot(Duck& duck) {
	Coordinates after_coordinates = StateManager::get_duck_coordinates(duck);
	if (duck.have_a_gun()) {
        guns_by_id.at(duck.get_gun_id())->updatePosition(after_coordinates.pos_X, after_coordinates.pos_Y);
        guns_by_id.at(duck.get_gun_id())->updateDirection(duck.is_moving_to_the_right(), duck.is_pointing_up());
        if (duck.shooting()) {
        	if (guns_by_id.at(duck.get_gun_id())->shoot(bullets_fired, bullets_by_id)) {
        		Coordinates bullet_position = bullets_by_id.back().second->getPosition();
        		int direction = guns_by_id.at(duck.get_gun_id())->is_pointing_to_the_right() ? 1 : 0;
        		Gamestate initial_bullet_coordinates(bullets_fired, guns_by_id.at(duck.get_gun_id())->getType(), direction,guns_by_id.at(duck.get_gun_id())->is_pointing_up(), bullet_position.pos_X, bullet_position.pos_Y);
        		players.broadcast(initial_bullet_coordinates);
        	}
        }else{
        	if (guns_by_id.at(duck.get_gun_id())->is_pew_pew_laser()) {
        		if (guns_by_id.at(duck.get_gun_id())->shoot(bullets_fired, bullets_by_id)) {
        			Coordinates bullet_position = bullets_by_id.back().second->getPosition();
        			int direction = guns_by_id.at(duck.get_gun_id())->is_pointing_to_the_right() ? 1 : 0;
        			Gamestate initial_bullet_coordinates(bullets_fired, guns_by_id.at(duck.get_gun_id())->getType(), direction,guns_by_id.at(duck.get_gun_id())->is_pointing_up(), bullet_position.pos_X, bullet_position.pos_Y);
        			players.broadcast(initial_bullet_coordinates);
        		}	
        	}
        	guns_by_id.at(duck.get_gun_id())->stopShoot();
        }
	}    
}

void Gameplay::try_to_slip_or_explode(Duck& duck, int duck_id) {
	Coordinates after_coordinates = StateManager::get_duck_coordinates(duck);
	for (auto& [id, gun] : guns_by_id) {
		if (gun->is_duck_position_valid(after_coordinates.pos_X, after_coordinates.pos_Y) && gun->is_banana_peel()) {
			duck.slip();
		}
		if (gun->is_a_grenade()) {
			if (gun->in_explosion_area(after_coordinates.pos_X, after_coordinates.pos_Y)) {
				std::cout<<"el pato exploto"<<std::endl;
				if (id == duck.get_gun_id()) {
					duck.drop_gun();
				}
                duck.set_is_NOT_alive();
                Gamestate duck_is_dead(StateManager::get_duck_state(duck, duck_id));
                broadcast_for_all_players(duck_is_dead);
			}
		}
	}
}

void Gameplay::send_guns_positions_updates(const unsigned int frame_delta) {
	std::map<int,std::pair<DrawingData, Coordinates>> guns_positions;
	std::list<int> guns_destroyed_id;
	for (auto& [id, gun] : guns_by_id) {
		if (!gun->isPickedUp()) {
			auto before_coordinates = gun->getPosition(); 
			gun->update_item_dropped_position(frame_delta);
			auto after_coordinates = gun->getPosition();
			terrain.adjust_position_for_collisions(gun, before_coordinates, after_coordinates);
		}
		if (gun->is_a_grenade()) {
			if (gun->is_destroyed()) {
				guns_destroyed_id.push_back(id);
				continue;
			}
			if (gun->try_to_explode_grenade()) {
				std::cout<<"la granada exploto"<<std::endl;
				Gamestate explosion(1, id, 1.0f);
				players.broadcast(explosion);
				//enviar mensaje de explosion
			}
		}
		DrawingData gun_data = {gun->getType(), gun->is_pointing_to_the_right() ? 1 : 0, gun->isShooting(), gun->is_pointing_up() ? 1 : 0};
		auto gun_position = gun->getPosition();
		guns_positions.insert({id, std::make_pair(gun_data, gun_position)});
	}	
	Gamestate update(guns_positions);
	players.broadcast(update);
	for (auto id : guns_destroyed_id) {
		guns_by_id.erase(id);
	}
}

void Gameplay::send_bullets_positions_updates(const unsigned int frame_delta) {
	if (!bullets_by_id.empty()) {
		std::map<int, Coordinates> bullets_positions;
		for (auto& [id,bullet] : bullets_by_id) {
			//Coordinates before_coordinates = bullet->getPosition();
			bullet->update_position(frame_delta);
			Coordinates after_coordinates = bullet->getPosition();
			/*if (not terrain.is_bullet_position_valid(after_coordinates.pos_X, after_coordinates.pos_Y)) {
				bullet.second->impact();
				Gamestate update(FLAG, bullet.first);
				players.broadcast(update);
				continue;
			//-----reemplazar por nueva colision
			}*/
			for (auto& [duck_id, duck] : ducks_by_id) {
                Coordinates duck_position = StateManager::get_duck_coordinates(duck);
                if (StateManager::get_duck_is_alive(duck) && duck_position.pos_X < after_coordinates.pos_X + 10 && duck_position.pos_X > after_coordinates.pos_X - 10 && duck_position.pos_Y < after_coordinates.pos_Y + 10 && duck_position.pos_Y > after_coordinates.pos_Y - 10) {
                    bullet->impact();
                    duck.set_is_NOT_alive();
                    Gamestate duck_is_dead(StateManager::get_duck_state(duck, duck_id));
                    broadcast_for_all_players(duck_is_dead);
                    break;
                }
            }
			if (bullet->is_destroyed()) {
				Gamestate update(id);
				players.broadcast(update);
				continue;
			}
			bullets_positions.insert({id, after_coordinates});
		}
		Gamestate update(1, bullets_positions);
		players.broadcast(update);
		bullets_by_id.remove_if([](auto& bullet){ return bullet.second->is_destroyed(); });
	}
}

void Gameplay::update_spawn_places() {
	for (auto& spawn_place : spawn_places) {
		if (spawn_place.try_spawn_gun(guns_in_map, guns_by_id)) {
			Gun* gun = guns_by_id[guns_in_map];
			Coordinates position = gun->getPosition();
    		Gamestate initial_gun_coordinates(
    			guns_in_map,
				position.pos_X,
            	position.pos_Y,
            	gun->getType(),
            	gun->is_pointing_to_the_right() ? 1 : 0
    		);
    		broadcast_for_all_players(initial_gun_coordinates);
			// std::cout<<"aparecio un arma con id: "<<guns_in_map<<"en la posicion x:"<<position.pos_X<<" y:"<<position.pos_Y<<std::endl;
		}
	}
}

void Gameplay::check_for_winner()
{
    std::vector<int> survivor_ids;
    for (auto& [id, duck] : ducks_by_id) {
        if (StateManager::get_duck_is_alive(duck) == 1) {
            survivor_ids.push_back(id);
        }
    }
    if (survivor_ids.size() == 1)
    {
        current_round++;
        round_is_over = true;
        scores_by_id.at(survivor_ids[0]) += 1;
        if (scores_by_id.at(survivor_ids[0]) >= SCORES_TO_WIN) {
            winner_id = survivor_ids[0];
            Gamestate winner(survivor_ids[0], 0);
            broadcast_for_all_players(winner);
        } else {
            Gamestate winner(survivor_ids[0], current_round);
            broadcast_for_all_players(winner);
        }
        ducks_by_id.clear();
        guns_by_id.clear();
        bullets_by_id.clear();
        spawn_places.clear();
        guns_in_map = 0;
        bullets_fired = 0;
    }
}

void Gameplay::run() {
    try
    {
        is_running.store(true);
        while (is_running.load()) {
            auto prev_time = std::chrono::steady_clock::now();
            initialize_players();
            send_all_initial_coordinates();
            round_is_over = false;
            while (not round_is_over) {
                auto current_time = std::chrono::steady_clock::now();
                auto frame_delta = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - prev_time).count();
                prev_time = current_time;
                process_users_commands();
                send_ducks_positions_updates(frame_delta);
                send_guns_positions_updates(frame_delta);
                send_bullets_positions_updates(frame_delta);
                update_spawn_places();
                check_for_winner();
                std::this_thread::sleep_for(std::chrono::milliseconds(17));
                if ((players.size() == 0) or (winner_id != -1)) {
                    is_running.store(false);
                    break;
                }
            }
        }
    }
    catch (ClosedQueue const& e)
    {
        std::cerr << "A queue was closed whilst in gameloop " << e.what() << '\n';
        is_running.store(false);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught in the gameplay thread: " << e.what() << '\n';
        is_running.store(false);
    }
    catch (...)
    {
        std::cerr << "Unknown exception on the gameloop.\n";
        is_running.store(false);
    }
}

void Gameplay::stop() {
    _keep_running = false;
    is_running.store(false);
}

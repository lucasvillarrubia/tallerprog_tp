#include <iostream>

#include "gameplay.h"

#include "common/message.h"
#include "server/state_manager.h"


Gameplay::Gameplay(MonitoredList<Player*>& player_list, Queue<Gameaction>& usr_cmds):
        is_running(false), players(player_list), user_commands(usr_cmds), bullets_fired(0) {
    // patos de prueba para el zoom
    Duck hugo, paco, luis;
    hugo.set_position(210.0f, 300.0f);
    paco.set_position(300.0f, 300.0f);
    luis.set_position(220.0f, 350.0f);
    luis.set_is_NOT_alive();
    ducks_by_id.insert({2, hugo});
    ducks_by_id.insert({3, paco});
    ducks_by_id.insert({4, luis});
}

void Gameplay::broadcast_for_all_players(const Gamestate& state)
{
    players.for_each([&state](Player* player) {
        player->add_message_to_queue(state);
    });
}

void Gameplay::send_all_initial_coordinates()
{
    // for (int i = 1; i <= players.size(); i++)
    // {
    //     if (i == 1) {
            float x = 300.0f;
            float y = 300.0f;
            Duck duck;
            duck.set_position(x, y);
            duck.set_is_on_the_floor();
            ducks_by_id.insert({1, duck});
            Gamestate initial_duck_coordinates(1, x, y, 0, 0, 0, 1, 1, 0.0f);
            broadcast_for_all_players(initial_duck_coordinates);
        // }
    // }
    for (auto& [id, duck]: ducks_by_id)
    {
        if (id != 1) {
            Coordinates position = StateManager::get_duck_coordinates(duck);
            Gamestate initial_duck_coordinates(
                id,
                position.pos_X,
                position.pos_Y,
                0,
                0,
                0,
                1,
                StateManager::get_duck_is_alive(duck),
                0.0f
            );
            broadcast_for_all_players(initial_duck_coordinates);
        }   
    }
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
}

void Gameplay::process_users_commands() {
    Gameaction command;
    while (user_commands.try_pop(command)) {
        StateManager::update_duck_state(ducks_by_id.at(command.player_id), command);
        Gamestate update = StateManager::get_duck_state(ducks_by_id.at(command.player_id), command.player_id);
        broadcast_for_all_players(update);
    }
}

void Gameplay::send_ducks_positions_updates(const unsigned int frame_delta)
{
    std::map<int, Coordinates> positions_by_id;
    for (auto& [id, duck]: ducks_by_id)
    {
        Coordinates before_coordinates = StateManager::get_duck_coordinates(duck);
        duck.update_position(frame_delta);
        Coordinates after_coordinates = StateManager::get_duck_coordinates(duck);
        terrain.adjust_position_for_collisions(duck, before_coordinates, after_coordinates);
        Coordinates updated_position = StateManager::get_duck_coordinates(duck);
        if ((id == 1) and ((updated_position.pos_Y != after_coordinates.pos_Y) or (updated_position.pos_X != after_coordinates.pos_X)))
        {
            std::cout << "x: " << updated_position.pos_X << " y: " << updated_position.pos_Y << "\n";
        }
        try_to_grab(duck);
        try_to_shoot(duck);
        positions_by_id.insert({id, updated_position});
    }
    Gamestate update(positions_by_id);
    broadcast_for_all_players(update);
}

void Gameplay::try_to_grab(Duck& duck) {
	Coordinates after_coordinates = StateManager::get_duck_coordinates(duck);
	if (duck.wants_to_grab()) {
		if (duck.have_a_gun()) {
        	duck.drop_gun();
        	duck.stop_grab();
        } else {
        	for (auto& [id,gun] : guns_by_id) {
        		if (gun->is_duck_position_valid(after_coordinates.pos_X, after_coordinates.pos_Y)) {
        			duck.pickup_gun(id);
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
        guns_by_id.at(duck.get_gun_id())->updateDirection(duck.is_moving_to_the_right());
        if (duck.shooting()) {
        	if (guns_by_id.at(duck.get_gun_id())->shoot(bullets_fired, bullets_by_id)) {
        		Coordinates bullet_position = bullets_by_id.back().second->getPosition();
        		int direction = guns_by_id.at(duck.get_gun_id())->is_pointing_to_the_right() ? 1 : 0;
        		Gamestate initial_bullet_coordinates(bullets_fired, guns_by_id.at(duck.get_gun_id())->getType(), direction, bullet_position.pos_X, bullet_position.pos_Y);
        		players.broadcast(initial_bullet_coordinates);
        	}
        }else{
        	guns_by_id.at(duck.get_gun_id())->stopShoot();
        }
	}    
}

void Gameplay::send_guns_positions_updates() {
	std::map<int,std::pair<DrawingData, Coordinates>> guns_positions;
	for (auto& [id, gun] : guns_by_id) {
		DrawingData gun_data = {gun->getType(), gun->is_pointing_to_the_right() ? 1 : 0};
		auto gun_position = gun->getPosition();
		guns_positions.insert({id, std::make_pair(gun_data, gun_position)});
	}	
	Gamestate update(guns_positions);
	players.broadcast(update);
}

void Gameplay::send_bullets_positions_updates(const unsigned int frame_delta) {
	std::map<int, Coordinates> bullets_positions;
	for (auto& bullet : bullets_by_id) {
		//Coordinates before_coordinates = bullet->getPosition();
		bullet.second->update_position(frame_delta);
		Coordinates after_coordinates = bullet.second->getPosition();
		/*if (not terrain.is_bullet_position_valid(after_coordinates.pos_X, after_coordinates.pos_Y)) {
			bullet.second->impact();
			Gamestate update(FLAG, bullet.first);
			players.broadcast(update);
			continue;
			//-----reemplazar por nueva colision
		}*/
		//if (colision contra pato){
			//duck.damage(bullet.impact)
		//}
		if (bullet.second->is_destroyed()) {
			Gamestate update(bullet.first);
			players.broadcast(update);
			continue;
		}
		bullets_positions.insert({bullet.first, after_coordinates});
	}
	Gamestate update(1, bullets_positions);
	players.broadcast(update);
	bullets_by_id.remove_if([](auto& bullet){ return bullet.second->is_destroyed(); });
}

void Gameplay::run() {
    try
    {
        is_running.store(true);
        auto prev_time = std::chrono::steady_clock::now();
        send_all_initial_coordinates();
        while (is_running.load()) {
            auto current_time = std::chrono::steady_clock::now();
            auto frame_delta = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - prev_time).count();
            prev_time = current_time;
            process_users_commands();
            send_ducks_positions_updates(frame_delta);
            send_guns_positions_updates();
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
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

#include <iostream>

#include "common/message.h"

#include "gameplay.h"

#include "server//state_manager.h"

#define FLAG 2


Gameplay::Gameplay(MonitoredList<Player*>& player_list, Queue<Gameaction>& usr_cmds):
        is_running(false), players(player_list), user_commands(usr_cmds) {
    ducks_by_id.insert({1, Duck()});
    ducks_by_id.insert({2, Duck()});
    //gun = new Magnum(125.0f, 50.0f);
    guns.insert({1, new Magnum(125.0f, 50.0f)});
    guns.insert({2, new DuelPistol(180.0f, 50.0f)});
    guns.insert({3, new CowboyPistol(245.0f, 50.0f)});
    guns.insert({4, new AK47(300.0f, 50.0f)});
    balas_disparadas = 0;
}

void Gameplay::process_users_commands() {
    if (primera_caida) {
        ducks_by_id.at(1).set_is_NOT_on_the_floor();
        primera_caida = false;
    }
    Gameaction command;
    while (user_commands.try_pop(command)) {
        StateManager::update_duck_state(ducks_by_id.at(command.player_id), command);
        Gamestate update = StateManager::get_duck_state(ducks_by_id.at(command.player_id), command.player_id);
        players.broadcast(update);
        // Coordinates pato = StateManager::get_duck_coordinates(ducks_by_id.at(command.player_id));
        // std::cout << "x: " << pato.pos_X << " y: " << pato.pos_Y << "\n";
        //float pato_velocidad = StateManager::get_duck_speed(ducks_by_id.at(command.player_id));
        //std::cout << "velocidad: " << pato_velocidad << "\n";
    }
}

void Gameplay::send_all_initial_coordinates()
{
    if ((players.size() == 1))
    {
        for (auto& [id, duck]: ducks_by_id)
        {
            if (id == 1)
            {
                float x = 125.0f;
                float y = 300.0f;
                Gamestate initial_duck_coordinates(id, x, y, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0.0f);
                duck.set_position(x, y);
                duck.set_is_on_the_floor();
                players.broadcast(initial_duck_coordinates);
            }
            else
            {
                Gamestate initial_duck_coordinates(id, 590.0f, 0.0f, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0.0f);
                players.broadcast(initial_duck_coordinates);
            }
        }
        //broadcast(initial_gun_coordinates)
        ya_entro_cliente = true;
    }
    //Gamestate initial_gun_coordinates(1,gun->getType(), gun->getPosition().pos_X, gun->getPosition().pos_Y);
	//players.broadcast(initial_gun_coordinates);
	for (auto& [id,gun]: guns) {
		Gamestate initial_gun_coordinates_2(id,gun->getType(), gun->getPosition().pos_X, gun->getPosition().pos_Y);
		players.broadcast(initial_gun_coordinates_2);
	
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
        if (not terrain.is_duck_position_valid(after_coordinates.pos_X, after_coordinates.pos_Y)) {
            duck.set_position(before_coordinates.pos_X, before_coordinates.pos_Y);
            duck.set_is_on_the_floor();
        }
        else {
            duck.set_is_NOT_on_the_floor();
        }
        if (duck.wants_to_grab()) {
        	if (duck.have_a_gun()) {
        		duck.drop_gun();
        		duck.stop_grab();
        	} else {
        		for (auto& [id,gun] : guns) {
        			if (gun->is_duck_position_valid(after_coordinates.pos_X, after_coordinates.pos_Y)) {
        				duck.pickup_gun(id);
        				duck.stop_grab();
        			}
        		}
        	}
        }
        if (duck.have_a_gun()) {
        	guns.at(duck.get_gun_id())->updatePosition(after_coordinates.pos_X, after_coordinates.pos_Y);
        	guns.at(duck.get_gun_id())->updateDirection(duck.is_moving_to_the_right());
        	if (duck.shooting()) {
        		if (guns.at(duck.get_gun_id())->shoot(balas_disparadas, bullets)) {
        			Coordinates bullet_position = bullets.back().second->getPosition();
        			Gamestate initial_bullet_coordinates(balas_disparadas, balas_disparadas, guns.at(duck.get_gun_id())->getType(), bullet_position.pos_X, bullet_position.pos_Y);
        			players.broadcast(initial_bullet_coordinates);
        		}
        	}else{
        		guns.at(duck.get_gun_id())->stopShoot();
        	}
        }
        positions_by_id.insert({id, StateManager::get_duck_coordinates(duck)});
    }
    Gamestate update(positions_by_id);
    players.broadcast(update);
    //std::cout << "x: " << positions_by_id.at(1).pos_X << " y: " << positions_by_id.at(1).pos_Y << "\n";
}

void Gameplay::send_guns_positions_updates() {
	std::map<int,std::pair<int, Coordinates>> guns_positions;
	//auto gun_position = std::make_pair(gun->getType(),gun->getPosition());
	//guns_positions[1] = gun_position;
	for (auto& [id, gun] : guns) {
		auto gun_position = std::make_pair(gun->getType(),gun->getPosition());
		guns_positions[id] = gun_position;
	}	
	Gamestate update(guns_positions);
	players.broadcast(update);
}


void Gameplay::send_bullets_positions_updates(const unsigned int frame_delta) {
	std::map<int, std::pair<int, Coordinates>> bullets_positions;
	for (auto& bullet : bullets) {
		//Coordinates before_coordinates = bullet->getPosition();
		bullet.second->update_position(frame_delta);
		Coordinates after_coordinates = bullet.second->getPosition();
		if (not terrain.is_bullet_position_valid(after_coordinates.pos_X, after_coordinates.pos_Y)) {
			bullet.second->impact();
			Gamestate update(FLAG, bullet.first);
			players.broadcast(update);
			//enviar coords de impacto
			continue;
		}
		//if (colision contra pato){
			//duck.damage(bullet.impact)
		//}
		if (bullet.second->is_destroyed()) {
			Gamestate update(FLAG, bullet.first);
			players.broadcast(update);
			continue;
		}
		bullets_positions.insert({bullet.first, std::make_pair(bullet.second->getType(), after_coordinates)});
	}
	Gamestate update(FLAG, bullets_positions);
	players.broadcast(update);
	bullets.remove_if([](auto& bullet){ return bullet.second->is_destroyed(); });
}

void Gameplay::run() {
    try
    {
        primera_caida = true;
        ya_entro_cliente = false;
        is_running.store(true);
        auto prev_time = std::chrono::steady_clock::now();
        send_all_initial_coordinates();
        while (is_running.load()) {
            if (not ya_entro_cliente)
                send_all_initial_coordinates();
            auto current_time = std::chrono::steady_clock::now();
            process_users_commands();
            auto frame_delta = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - prev_time).count();
            prev_time = current_time;
            send_ducks_positions_updates(frame_delta);
            send_guns_positions_updates();
            if (bullets.size()>0) {
            	send_bullets_positions_updates(frame_delta);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Maso 60 FPS
        }
    }
    catch (ClosedQueue const& e)
    {
        std::cerr << "Se cerró la queue del juego?! " << e.what() << '\n';
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

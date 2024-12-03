#include "Grenade.h"

#include <iostream>

Grenade::Grenade(float x, float y, std::map<std::string, float> config) : 
	Gun(x,y,config["DIMENSION_X"],config["DIMENSION_Y"],static_cast<int>(config["TYPE"])),
	activated(false),
	explosion_area(positionX-(config["EXPLOSION_RADIUS"]),
	positionY+config["EXPLOSION_RADIUS"], 2*config["EXPLOSION_RADIUS"], 2*config["EXPLOSION_RADIUS"]),
	detonation_time(config["DETONATION_TIME"]),
	detonated(false) {}

void Grenade::updatePosition(float x, float y){
	positionX = x;
	positionY = y;
	hitbox.update_position(x,y);
	explosion_area.update_position(x,y);
}

bool Grenade::is_duck_position_valid(float x, float y)
{
    Rectangulo character(x, y, 64.0f, 64.0f);
    return hitbox.hay_colision(character);
}

bool Grenade::shoot([[maybe_unused]] int& id, [[maybe_unused]] std::list<std::pair<int, Ammo*>>& bullets) {
	if (!activated) {
		activated = true;
		activation_time = std::chrono::steady_clock::now();
	}
	return false;
}

bool Grenade::is_a_grenade() {
	return true;
}

bool Grenade::in_explosion_area(float x, float y) {
	if (detonated) {
		Rectangulo character(x, y, 64.0f, 64.0f);
    	return explosion_area.hay_colision(character);
	}
	return false;
}

bool Grenade::isShooting() {
	return activated;
}


bool Grenade::try_to_explode_grenade() {
	if (activated && !detonated) {
		auto time_now = std::chrono::steady_clock::now();
		auto t = std::chrono::duration_cast<std::chrono::seconds> (time_now-activation_time).count();
		if (t>=detonation_time) {
			detonated = true;
			return true;
		}
	}
	return false;
}

bool Grenade::is_destroyed() {
	return detonated;
}

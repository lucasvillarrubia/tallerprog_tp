#include "Sniper.h"

Sniper::Sniper(float x, float y, std::map<std::string, float> config) : 
	Gun(x,y,config["DIMENSION_X"],config["DIMENSION_Y"],static_cast<int>(config["TYPE"])),
	ammo(static_cast<int>(config["AMMO"])),
	time_between_shots(config["TIME_BETWEEN_SHOTS"]) {
	last_shot_time = std::chrono::steady_clock::now();
}

bool Sniper::is_duck_position_valid(float x, float y)
{
    Rectangulo character(x, y, 64.0f, 64.0f);
    return hitbox.hay_colision(character);
}

bool Sniper::shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets) {
	
	auto time_now = std::chrono::steady_clock::now();
	auto t = std::chrono::duration_cast<std::chrono::seconds> (time_now-last_shot_time).count();
	if (!shooting && ammo > 0 && (t) >= time_between_shots) {
		last_shot_time = time_now;
		int dir = rightDirection ? 36 : -12;
		int dir_y = pointing_up ? 36 : 0;
		dir = pointing_up ? 24 : dir;
		shooting = true;
		ammo--;
		id++;
		bullets.push_back(std::make_pair(id, new SniperAmmo(positionX+dir, positionY+dir_y,rightDirection, pointing_up)));
		return true;
	}
	return false;
}

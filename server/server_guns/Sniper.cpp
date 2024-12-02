#include "Sniper.h"

Sniper::Sniper(float x, float y) : Gun(x,y,32,32,10), ammo(3), time_between_shots(1.5) {
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
		shooting = true;
		ammo--;
		id++;
		bullets.push_back(std::make_pair(id, new SniperAmmo(positionX+dir, positionY,rightDirection, pointing_up)));
		return true;
	}
	return false;
}

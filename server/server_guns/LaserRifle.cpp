#include "LaserRifle.h"

LaserRifle::LaserRifle(float x, float y): Gun(x,y,32,32,4), ammo(30), time_between_shots(0.5) {
	last_shot_time = std::chrono::steady_clock::now(); 
}

bool LaserRifle::shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets) {
	auto time_now = std::chrono::steady_clock::now();
	auto t = std::chrono::duration_cast<std::chrono::milliseconds> (time_now-last_shot_time).count(); 
	if (ammo > 0 && (t) >= time_between_shots) {
		last_shot_time = time_now;
		int dir = rightDirection ? 36 : -12;
		int dir_y = pointing_up ? 36 : 0;
		dir = pointing_up ? 24 : dir;
		ammo--;
		id++;
		bullets.push_back(std::make_pair(id, new LaserRifleAmmo(positionX+dir, positionY+dir_y, rightDirection, pointing_up)));
		return true;
	}
	return false;
}

bool LaserRifle::is_duck_position_valid(float x, float y)
{
    Rectangulo character(x, y, 64.0f, 64.0f);
    return hitbox.hay_colision(character);
}

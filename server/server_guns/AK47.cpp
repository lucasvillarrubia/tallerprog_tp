#include "AK47.h"

AK47::AK47(float x, float y): Gun(x,y,32,32,5), ammo(30), time_between_shots(0.5) {
	last_shot_time = std::chrono::steady_clock::now(); 
}

bool AK47::shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets) {
	auto time_now = std::chrono::steady_clock::now();
	auto t = std::chrono::duration_cast<std::chrono::milliseconds> (time_now-last_shot_time).count(); 
	if (ammo > 0 && (t) >= time_between_shots) { //actualmente funciona como arma semiautomatica
		last_shot_time = time_now;
		int dir = rightDirection ? 36 : -12;
		ammo--;
		id++;
		bullets.push_back(std::make_pair(id, new AK47Ammo(positionX+dir, positionY, rightDirection, pointing_up)));
		return true;
	}
	return false;
}

bool AK47::is_duck_position_valid(float x, float y)
{
    Rectangulo character(x, y, 64.0f, 64.0f);
    return hitbox.hay_colision(character);
}

#include "CowboyPistol.h"

CowboyPistol::CowboyPistol(float x, float y, std::map<std::string, float> config): 
	Gun(x,y,config["DIMENSION_X"],config["DIMENSION_Y"],static_cast<int>(config["TYPE"])), ammo(static_cast<int>(config["AMMO"])) {}

bool CowboyPistol::shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets) {
	if (!shooting && (ammo > 0)) {
		int dir = rightDirection ? 36 : -12;
		int dir_y = pointing_up ? 36 : 0;
		dir = pointing_up ? 24 : dir;
		shooting = true;
		ammo--;
		id++;
		bullets.push_back(std::make_pair(id, new CowboyPistolAmmo(positionX+dir, positionY+dir_y, rightDirection, pointing_up)));
		return true;
	}
	return false;
}

bool CowboyPistol::is_duck_position_valid(float x, float y)
{
    Rectangulo character(x, y, 64.0f, 64.0f);
    return hitbox.hay_colision(character);
}

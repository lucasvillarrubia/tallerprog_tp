#include "DuelPistol.h"

DuelPistol::DuelPistol(float x, float y): Gun(x,y,32,32,6), ammo(1) {}

bool DuelPistol::shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets) {
	if (!shooting && (ammo > 0)) {
		int dir = rightDirection ? 36 : -12;
		shooting = true;
		ammo--;
		id++;
		bullets.push_back(std::make_pair(id, new DuelPistolAmmo(positionX+dir, positionY, rightDirection)));
		return true;
		
	}
	return false;
}

bool DuelPistol::is_duck_position_valid(float x, float y)
{
    Rectangulo character(x, y, 64.0f, 64.0f);
    return hitbox.hay_colision(character);
}

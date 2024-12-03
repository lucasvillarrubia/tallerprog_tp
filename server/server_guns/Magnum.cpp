#include "Magnum.h"

#include "ammo.h"

Magnum::Magnum(float x, float y): Gun(x,y,64,64, 8), ammo(6) {}

bool Magnum::shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets) {
	if (!shooting && (ammo > 0)) {
		int dir = rightDirection ? 36 : -12;
		int dir_y = pointing_up ? 36 : 0;
		dir = pointing_up ? 24 : dir;
		shooting = true;
		ammo--;
		id++;
		bullets.push_back(std::make_pair(id, new MagnumAmmo(positionX+dir, positionY+dir_y, rightDirection, pointing_up)));
		return true;
	}
	return false;
}

bool Magnum::is_duck_position_valid(float x, float y)
{
    Rectangulo character(x, y, 64.0f, 64.0f);
    return hitbox.hay_colision(character);
}

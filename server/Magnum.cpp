#include "Magnum.h"

#include "ammo.h"

Magnum::Magnum(float x, float y): Gun(x,y,32,32, 8), ammo(60) {}
/*
MagnumAmmo Magnum::shoot() {
	if (!shooting && pickedUp && (ammo > 0)) {
		int dir = rightDirection ? 36 : -12;
		shooting = true;
		ammo--;
		return MagnumAmmo(positionX+dir, positionY, rightDirection);
	}
	return MagnumAmmo();
}*/

bool Magnum::shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets) {
	if (!shooting && (ammo > 0)) {
		int dir = rightDirection ? 36 : -12;
		shooting = true;
		ammo--;
		id++;
		bullets.push_back(std::make_pair(id, new MagnumAmmo(positionX+dir, positionY, rightDirection)));
		return true;
	}
	return false;
}

bool Magnum::is_duck_position_valid(float x, float y)
{
    Rectangulo character(x, y, 64.0f, 64.0f);
    return hitbox.hay_colision(character);
}

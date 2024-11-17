#include "Magnum.h"

Magnum::Magnum(float x, float y): Gun(x,y,32,32, 8), ammo(6) {}

MagnumAmmo Magnum::shoot() {
	if (!shooting && pickedUp && (ammo > 0)) {
		int dir = rightDirection ? 36 : -12;
		shooting = true;
		ammo--;
		return MagnumAmmo(positionX+dir, positionY, rightDirection);
	}
	return MagnumAmmo();
}

bool Magnum::is_duck_position_valid(float x, float y)
{
    Rectangulo character(x, y, 64.0f, 64.0f);
    return hitbox.hay_colision(character);
}

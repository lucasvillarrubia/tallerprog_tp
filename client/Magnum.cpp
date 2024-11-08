#include "Magnum.h"

Magnum::Magnum(float x, float y): Gun(x,y), ammo(6) {}

MagnumAmmo Magnum::shoot() {
	if (!shooting && pickedUp && (ammo > 0)) {
		int dir = rightDirection ? 36 : -12;
		shooting = true;
		ammo--;
		return MagnumAmmo(positionX+dir, positionY, rightDirection);
	}
	return MagnumAmmo();
}


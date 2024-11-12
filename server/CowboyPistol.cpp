#include "CowboyPistol.h"

CowboyPistol::CowboyPistol(float x, float y): Gun(x,y), ammo(6) {}

CowboyPistolAmmo CowboyPistol::shoot() {
	if (!shooting && pickedUp && (ammo > 0)) {
		int dir = rightDirection ? 36 : -12;
		shooting = true;
		ammo--;
		return CowboyPistolAmmo(positionX+dir, positionY, rightDirection);
	}
	return CowboyPistolAmmo();
}


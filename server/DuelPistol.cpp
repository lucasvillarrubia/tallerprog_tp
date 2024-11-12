#include "DuelPistol.h"

DuelPistol::DuelPistol(float x, float y): Gun(x,y), ammo(1) {}

DuelPistolAmmo DuelPistol::shoot() {
	if (!shooting && pickedUp && (ammo > 0)) {
		int dir = rightDirection ? 36 : -12;
		shooting = true;
		ammo--;
		return DuelPistolAmmo(positionX+dir, positionY, rightDirection);
	}
	return DuelPistolAmmo();
}


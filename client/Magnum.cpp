#include "Magnum.h"

Magnum::Magnum(float x, float y): positionX(x), positionY(y), shooting(false), pickedUp(false), ammo(6) {}

void Magnum::updatePosition(float x, float y){
	positionX = x;
	positionY = y;
}

Coordinates Magnum::getPosition() {
	return { positionX, positionY};
}

void Magnum::collected() {
	pickedUp = true;
}

bool Magnum::isShooting() {
	return shooting;
}

bool Magnum::isPickedUp() {
	return pickedUp;
}

MagnumAmmo Magnum::shoot() {
	if (!shooting && pickedUp && (ammo > 0)) {
		shooting = true;
		ammo--;
		return MagnumAmmo(positionX, positionY);
	}
	return MagnumAmmo(true);
}

void Magnum::stopShoot() {
	shooting = false;
}

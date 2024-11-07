#include "Magnum.h"

Magnum::Magnum(float x, float y): positionX(x), positionY(y), shooting(false), pickedUp(false), ammo(6) {}

void Magnum::updatePosition(float x, float y){
	positionX = x;
	positionY = y;
}

Coordinates Magnum::getPosition() {
	return { positionX, positionY};
}

float Magnum::getPositionX() {
	return positionX;
}

float Magnum::getPositionY() {
	return positionY;
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

void Magnum::shoot() {
	if (!shooting) {
		shooting = true;
	}
}

void Magnum::stopShoot() {
	shooting = false;
}

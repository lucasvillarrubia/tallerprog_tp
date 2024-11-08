#include "gun.h"

Gun::Gun(float x, float y) : positionX(x), positionY(y), shooting(false), pickedUp(false), rightDirection(true) {}

void Gun::updatePosition(float x, float y){
	positionX = x;
	positionY = y;
}

void Gun::updateDirection(bool right) {
	rightDirection = right;
}

Coordinates Gun::getPosition() {
	return { positionX, positionY};
}

void Gun::collected() {
	pickedUp = true;
}

bool Gun::isShooting() {
	return shooting;
}

bool Gun::isPickedUp() {
	return pickedUp;
}

void Gun::stopShoot() {
	shooting = false;
}


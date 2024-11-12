#include "item.h"

Item::Item(float x, float y) : positionX(x), positionY(y), pickedUp(false), rightDirection(true) {}

void Item::updatePosition(float x, float y){
	positionX = x;
	positionY = y;
}

void Item::updateDirection(bool right) {
	rightDirection = right;
}

Coordinates Item::getPosition() {
	return { positionX, positionY};
}

void Item::collected() {
	pickedUp = true;
}

bool Item::isPickedUp() {
	return pickedUp;
}


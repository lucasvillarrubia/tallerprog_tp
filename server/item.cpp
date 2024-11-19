#include "item.h"

Item::Item(float x, float y, float width, float height, int type) : positionX(x), positionY(y), pickedUp(false), itemType(type), rightDirection(true), hitbox(x, y, width, height) {}

void Item::updatePosition(float x, float y){
	positionX = x;
	positionY = y;
	hitbox.x = x;
	hitbox.y = y;
}

void Item::updateDirection(bool right) {
	rightDirection = right;
}

Coordinates Item::getPosition() {
	return { positionX, positionY};
}

int Item::getType() {
	return itemType;
}

void Item::collected() {
	pickedUp = true;
}

bool Item::isPickedUp() {
	return pickedUp;
}

bool Item::is_pointing_to_the_right() const {
	return rightDirection;
}

#include "ammo.h"

Ammo::Ammo(float x, float y, bool right) : originX(x), originY(y), positionX(originX), positionY(originY), destroyed(false), movingRight(right) {}

bool Ammo::isDestroyed() {
	return destroyed;
}

Coordinates Ammo::getPosition() {
	return { positionX, positionY};
}

bool Ammo::isMovingRight() {
	return movingRight;
}

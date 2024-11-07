#include "MagnumAmmo.h"

MagnumAmmo::MagnumAmmo(float x, float y) : originX(x), originY(y), positionX(originX), positionY(originY), damagePoints(10), scope(20*16), destroyed(false) {}

MagnumAmmo::MagnumAmmo(bool destroy) : originX(0), originY(0), positionX(originX), positionY(originY), damagePoints(10), scope(20), destroyed(destroy){}

void MagnumAmmo::updatePosition(const unsigned int frame_delta) {
	if (!destroyed){
		positionX+= frame_delta * 0.3;
		if (abs(positionX-originX) > scope) {
			destroyed = true;
		}
	}
}

float MagnumAmmo::impact() {
	destroyed = true;
	return damagePoints;
}

bool MagnumAmmo::isDestroyed() {
	return destroyed;
}

Coordinates MagnumAmmo::getPosition() {
	return { positionX, positionY};
}


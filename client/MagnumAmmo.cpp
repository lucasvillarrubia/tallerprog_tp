#include "MagnumAmmo.h"

MagnumAmmo::MagnumAmmo(float x, float y, bool right) : Ammo(x,y,right), damagePoints(10), scope(20*16) {}

MagnumAmmo::MagnumAmmo() :  Ammo(0, 0, true), damagePoints(10), scope(20) {
	destroyed = true;
}

void MagnumAmmo::updatePosition(const unsigned int frame_delta)  {
	if (!destroyed){
		if (movingRight) {
			positionX += frame_delta * 0.3;
		} else {
			positionX -= frame_delta * 0.3;
		}
		if (abs(positionX-originX) > scope) {
			destroyed = true;
		}
	}
}

float MagnumAmmo::impact() {
	destroyed = true;
	return damagePoints;
}


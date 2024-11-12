#include "AK47Ammo.h"

AK47Ammo::AK47Ammo(float x, float y, bool right) : Ammo(x,y,right), damagePoints(10), scope(13*16) {}

AK47Ammo::AK47Ammo() :  Ammo(0, 0, true), damagePoints(10), scope(20) {
	destroyed = true;
}

void AK47Ammo::updatePosition(const unsigned int frame_delta)  {
	if (!destroyed){
		if (movingRight) {
			positionX += frame_delta * 0.4;
		} else {
			positionX -= frame_delta * 0.4;
		}
		if (abs(positionX-originX) > scope) {
			destroyed = true;
		}
	}
}

float AK47Ammo::impact() {
	destroyed = true;
	return damagePoints;
}


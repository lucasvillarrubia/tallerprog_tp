#include "AK47Ammo.h"

AK47Ammo::AK47Ammo(float x, float y, bool right, bool up) : Ammo(x,y,right,up,5,13*64), damagePoints(10), scope(13*64) {}

void AK47Ammo::update_position(const unsigned int frame_delta)  {
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

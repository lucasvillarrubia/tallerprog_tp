#include "PewPewLaserAmmo.h"

PewPewLaserAmmo::PewPewLaserAmmo(float x, float y, bool right, bool up) : Ammo(x,y,right,up,3,38*64), scope(38*64) {}

void PewPewLaserAmmo::update_position(const unsigned int frame_delta) {
	if (!destroyed){
		if (moving_up) {
			positionY += frame_delta * 0.5;
		} else if (movingRight) {
			positionX += frame_delta * 0.5;
		} else {
			positionX -= frame_delta * 0.5;
		}
		if (abs(positionX-originX) > scope) {
			destroyed = true;
		}
	}
}

float PewPewLaserAmmo::impact() {
	destroyed = true;
	return 0;
}

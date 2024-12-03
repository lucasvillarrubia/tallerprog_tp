#include "MagnumAmmo.h"

MagnumAmmo::MagnumAmmo(float x, float y, bool right, bool up) : Ammo(x,y,right,up, 8, 20*16), damagePoints(10) {}

void MagnumAmmo::update_position(const unsigned int frame_delta)  {
	if (!this->destroyed){
		if (moving_up) {
			positionY += frame_delta * 0.4;
		} else if (movingRight) {
			positionX += frame_delta * 0.4;
		} else {
			positionX -= frame_delta * 0.4;
		}
		if (abs(positionX-originX) > scope) {
			this->destroyed = true;
		}
	}
}

float MagnumAmmo::impact() {
	destroyed = true;
	return damagePoints;
}


#include "MagnumAmmo.h"

MagnumAmmo::MagnumAmmo(float x, float y, bool right) : Ammo(x,y,right, 8, 20*16), damagePoints(10) {}

void MagnumAmmo::update_position(const unsigned int frame_delta)  {
	if (!this->destroyed){
		if (movingRight) {
			positionX += frame_delta * 0.3;
		} else {
			positionX -= frame_delta * 0.3;
		}
		if (abs(positionX-originX) > scope) {
			this->destroyed = true;
		}
	}
}

bool MagnumAmmo::is_destroyed() {
	return this->destroyed;
}

float MagnumAmmo::impact() {
	destroyed = true;
	return damagePoints;
}


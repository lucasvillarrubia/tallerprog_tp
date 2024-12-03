#include "ShotgunAmmo.h"

ShotgunAmmo::ShotgunAmmo(float x, float y, bool right, bool up) : Ammo(x,y,right,up, 9, 20*16), damagePoints(7) {}

void ShotgunAmmo::update_position(const unsigned int frame_delta)  {
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

float ShotgunAmmo::impact() {
	destroyed = true;
	return damagePoints;
}


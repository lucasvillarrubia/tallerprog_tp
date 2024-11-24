#include "SniperAmmo.h"

SniperAmmo::SniperAmmo(float x, float y, bool right) : Ammo(x,y,right,10, 64*64), damagePoints(10) {}

void SniperAmmo::update_position(const unsigned int frame_delta)  {
	if (!destroyed){
		if (movingRight) {
			positionX += frame_delta * 0.7;
		} else {
			positionX -= frame_delta * 0.7;
		}
		if (abs(positionX-originX) > scope) {
			destroyed = true;
		}
	}
}

float SniperAmmo::impact() {
	destroyed = true;
	return damagePoints;
}

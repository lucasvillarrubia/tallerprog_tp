#include "LaserRifleAmmo.h"

LaserRifleAmmo::LaserRifleAmmo(float x, float y, bool right, bool up) : Ammo(x,y,right,up,5,38*64), damagePoints(10), scope(38*64) {}

void LaserRifleAmmo::update_position(const unsigned int frame_delta)  {
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

float LaserRifleAmmo::impact() {
	destroyed = true;
	return damagePoints;
}

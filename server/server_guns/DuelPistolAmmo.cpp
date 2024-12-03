#include "DuelPistolAmmo.h"

DuelPistolAmmo::DuelPistolAmmo(float x, float y, bool right, bool up) : Ammo(x,y,right,up, 6, 5*16), damagePoints(10) {}


void DuelPistolAmmo::update_position(const unsigned int frame_delta)  {
	if (!destroyed){
		if (moving_up) {
			positionY += frame_delta * 0.4;
		} else if (movingRight) {
			positionX += frame_delta * 0.4;
		} else {
			positionX -= frame_delta * 0.4;
		}
		if (abs(positionX-originX) > scope) {
			destroyed = true;
		}
	}
}

float DuelPistolAmmo::impact() {
	destroyed = true;
	return damagePoints;
}


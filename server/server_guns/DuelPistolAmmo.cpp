#include "DuelPistolAmmo.h"

DuelPistolAmmo::DuelPistolAmmo(float x, float y, bool right, bool up) : Ammo(x,y,right,up, 6, 5*16), damagePoints(10) {}


void DuelPistolAmmo::update_position(const unsigned int frame_delta)  {
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

float DuelPistolAmmo::impact() {
	destroyed = true;
	return damagePoints;
}


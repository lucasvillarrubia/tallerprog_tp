#include "CowboyPistolAmmo.h"

CowboyPistolAmmo::CowboyPistolAmmo(float x, float y, bool right) : Ammo(x,y,right, 7, 20*16), damagePoints(7) {}

void CowboyPistolAmmo::update_position(const unsigned int frame_delta)  {
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

float CowboyPistolAmmo::impact() {
	destroyed = true;
	return damagePoints;
}


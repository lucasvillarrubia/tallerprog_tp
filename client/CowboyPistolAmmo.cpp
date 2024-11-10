#include "CowboyPistolAmmo.h"

CowboyPistolAmmo::CowboyPistolAmmo(float x, float y, bool right) : Ammo(x,y,right), damagePoints(7), scope(20*16) {}

CowboyPistolAmmo::CowboyPistolAmmo() :  Ammo(0, 0, true), damagePoints(7), scope(20) {
	destroyed = true;
}

void CowboyPistolAmmo::updatePosition(const unsigned int frame_delta)  {
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


#include "ammo.h"

Ammo::Ammo(float x, float y, bool right, int type, float _scope) : originX(x), originY(y), positionX(originX), positionY(originY), destroyed(false), movingRight(right), type_gun(type), scope(_scope) {}

void Ammo::update_position(const unsigned int frame_delta)  {
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

float Ammo::impact() {
	destroyed = true;
	return 0;
}

bool Ammo::is_destroyed() {
	return destroyed;
}

Coordinates Ammo::getPosition() {
	return { positionX, positionY};
}

bool Ammo::isMovingRight() {
	return movingRight;
}

int Ammo::getType() {
	return type_gun;
}

#include "ammo.h"

Ammo::Ammo(float x, float y, bool right, bool up, int type, float _scope) : originX(x), originY(y), positionX(originX), positionY(originY), destroyed(false), movingRight(right), moving_up(up), type_gun(type), scope(_scope) {}

void Ammo::update_position([[maybe_unused]] const unsigned int frame_delta)  {}

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

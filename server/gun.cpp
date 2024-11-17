#include "gun.h"

Gun::Gun(float x, float y, float width, float height, int type) : Item(x, y, width, height, type), shooting(false) {}

bool Gun::isShooting() {
	return shooting;
}

void Gun::stopShoot() {
	shooting = false;
}


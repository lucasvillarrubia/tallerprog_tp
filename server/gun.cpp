#include "gun.h"

Gun::Gun(float x, float y) : Item(x,y), shooting(false) {}

bool Gun::isShooting() {
	return shooting;
}

void Gun::stopShoot() {
	shooting = false;
}


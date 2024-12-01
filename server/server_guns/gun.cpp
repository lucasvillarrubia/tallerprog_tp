#include "gun.h"

Gun::Gun(float x, float y, float width, float height, int type) : Item(x, y, width, height, type), shooting(false) {}

bool Gun::is_duck_position_valid(float x, float y)
{
    Rectangulo character(x, y, 64.0f, 64.0f);
    return hitbox.hay_colision(character);
}

bool Gun::isShooting() {
	return shooting;
}

bool Gun::shoot([[maybe_unused]] int& id, [[maybe_unused]] std::list<std::pair<int, Ammo*>>& bullets) {
	std::cout<<"disparo desde gun"<<std::endl;
	return false;
}

void Gun::stopShoot() {
	shooting = false;
}

bool Gun::is_banana_peel() {
	return false;
}

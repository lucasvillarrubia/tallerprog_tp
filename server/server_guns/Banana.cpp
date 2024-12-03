#include "Banana.h"

Banana::Banana(float x, float y, std::map<std::string, float> config) : 
	Gun(x,y,config["DIMENSION_X"],config["DIMENSION_Y"],static_cast<int>(config["TYPE"])), banana_peel(false) {}

void Banana::dropped() {
	pickedUp = false;
	is_dropped = true;
	is_on_the_floor = false;
	banana_peel = true;
	shooting = true;
}

bool Banana::is_duck_position_valid(float x, float y)
{
    Rectangulo character(x, y, 64.0f, 64.0f);
    return hitbox.hay_colision(character);
}

bool Banana::is_banana_peel() {
	return (banana_peel && is_on_the_floor);
}


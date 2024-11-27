#include "Banana.h"

Banana::Banana(float x, float y) : Gun(x,y,32,32,1), launched(false) {}

void Banana::launch() {
	launched = true;
	pickedUp = false;
	//vertical_velocity = 0.8f;
	//return bananaLaunched(...)
}

bool Banana::is_duck_position_valid(float x, float y)
{
    Rectangulo character(x, y, 64.0f, 64.0f);
    return hitbox.hay_colision(character);
}

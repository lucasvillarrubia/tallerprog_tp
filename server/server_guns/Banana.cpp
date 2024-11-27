#include "Banana.h"

Banana::Banana(float x, float y) : Item(x,y,32,32,1), launched(false) {}

void Banana::launch() {
	launched = true;
	picked_up = false;
	//vertical_velocity = 0.8f;
	//return bananaLaunched(...)
}

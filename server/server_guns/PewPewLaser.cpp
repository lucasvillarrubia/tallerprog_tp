#include "PewPewLaser.h"

PewPewLaser::PewPewLaser(float x, float y) : Gun(x,y,32,32,3), ammo(12), time_between_shots(0.5), shots_remaining(3) {
	last_shot_time = std::chrono::steady_clock::now();
}

bool PewPewLaser::shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets) {
	if ((ammo > 0) && (shots_remaining>0)) {
		shooting = true;
		shots_remaining--;
	}
}

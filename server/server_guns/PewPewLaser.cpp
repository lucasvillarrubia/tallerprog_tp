#include "PewPewLaser.h"

PewPewLaser::PewPewLaser(float x, float y) : Gun(x,y,32,32,3), ammo(12), time_between_shots(1), shots_remaining(3), burst(false) {
	last_shot_time = std::chrono::steady_clock::now();
}

bool PewPewLaser::shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets) {
	if ((!shooting) && (ammo > 0)) {
		shooting = true;
		ammo--;
		burst = true;
		shots_remaining = 3;
	}
	if (burst && shots_remaining>0) {
		auto time_now = std::chrono::steady_clock::now();
		auto t = std::chrono::duration_cast<std::chrono::milliseconds> (time_now-last_shot_time).count();
		if (t >= time_between_shots) {
			last_shot_time = time_now;
			int dir = rightDirection ? 36 : -12;
			shots_remaining--;
			id++;
			bullets.push_back(std::make_pair(id, new PewPewLaserAmmo(positionX+dir, positionY, rightDirection, pointing_up)));
			burst = (shots_remaining > 0);
			return true;
		}
	}
	return false;
}

bool PewPewLaser::is_pew_pew_laser() {
	return burst;
}

void PewPewLaser::stopShoot() {
	if (shots_remaining == 0) {
		shooting = false;
	}
}

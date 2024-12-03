#include "PewPewLaser.h"

PewPewLaser::PewPewLaser(float x, float y, std::map<std::string, float> config) : 
	Gun(x,y,config["DIMENSION_X"],config["DIMENSION_Y"],static_cast<int>(config["TYPE"])),
	ammo(static_cast<int>(config["AMMO"])),
	time_between_shots(config["TIME_BETWEEN_SHOTS"]), 
	burst_shots(config["BURST_SHOTS"]), 
	shots_remaining(0),
	burst(false) {
		last_shot_time = std::chrono::steady_clock::now();
}

bool PewPewLaser::shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets) {
	if ((!shooting) && (ammo > 0)) {
		shooting = true;
		ammo--;
		burst = true;
		shots_remaining = burst_shots;
	}
	if (burst && shots_remaining>0) {
		auto time_now = std::chrono::steady_clock::now();
		auto t = std::chrono::duration_cast<std::chrono::milliseconds> (time_now-last_shot_time).count();
		if (t >= time_between_shots) {
			last_shot_time = time_now;
			int dir = rightDirection ? 36 : -12;
			int dir_y = pointing_up ? 36 : 0;
			dir = pointing_up ? 24 : dir;
			shots_remaining--;
			id++;
			bullets.push_back(std::make_pair(id, new PewPewLaserAmmo(positionX+dir, positionY+dir_y, rightDirection, pointing_up)));
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

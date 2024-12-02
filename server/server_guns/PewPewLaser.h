#ifndef PEWPEWLASER_H
#define PEWPEWLASER_H

#include "gun.h"
#include "ammo.h"
#include "PewPewLaserAmmo.h"

#include <chrono>

class PewPewLaser : public Gun {
private:
	int ammo;
	const float time_between_shots;
	int shots_remaining;
	std::chrono::time_point<std::chrono::_V2::steady_clock> last_shot_time;
	bool burst;
public:
	PewPewLaser(float x, float y);
	bool shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets) override;
	bool is_duck_position_valid(float x, float y);
	bool is_pew_pew_laser() override;
	void stopShoot() override;
};

#endif

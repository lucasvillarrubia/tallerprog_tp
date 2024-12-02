#ifndef LASERRIFLE_H
#define LASERRIFLE_H

#include "gun.h"
#include "ammo.h"
#include "LaserRifleAmmo.h"

#include <chrono>

class LaserRifle : public Gun {
private:	
	int ammo;
	const float time_between_shots;
	std::chrono::time_point<std::chrono::_V2::steady_clock> last_shot_time;
public:
	LaserRifle(float x, float y);
	bool shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets) override;
	bool is_duck_position_valid(float x, float y);
};

#endif

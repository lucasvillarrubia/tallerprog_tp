#ifndef SNIPER_H
#define SNIPER_H


#include <cmath>
#include <chrono>
#include <list>
#include <utility>

#include "ammo.h"
#include "gun.h"
#include "SniperAmmo.h"

class Sniper : public Gun {
	int ammo;
	
	const float time_between_shots;
	std::chrono::time_point<std::chrono::_V2::steady_clock> last_shot_time;
public:
	Sniper(float x, float y);
	bool is_duck_position_valid(float x, float y);
	bool shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets) override;
};

#endif

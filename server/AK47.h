#ifndef AK47_H
#define AK47_H

#include "gun.h"
#include "common/coordinates.h"
#include "AK47Ammo.h"
#include "ammo.h"

#include <cmath>
#include <chrono>
#include <list>
#include <utility>

class AK47 : public Gun {
private:
	int ammo;
	//variables para medir el tiempo entre disparo
	const float time_between_shots;
	std::chrono::time_point<std::chrono::_V2::steady_clock> last_shot_time;
public:
	AK47(float x, float y);
	bool shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets) override;
	bool is_duck_position_valid(float x, float y);
};

#endif

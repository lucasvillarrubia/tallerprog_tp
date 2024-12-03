#ifndef GRENADE_H
#define GRENADE_H

#include "gun.h"
#include "ammo.h"

#include <chrono>
#include <map>
#include <string>

class Grenade : public Gun {
private:
	bool activated;
	Rectangulo explosion_area;
	const float detonation_time;
	std::chrono::time_point<std::chrono::_V2::steady_clock> activation_time;
	bool detonated;
public:
	Grenade(float x, float y, std::map<std::string, float> config);
	void updatePosition(float x, float y) override;
	bool is_duck_position_valid(float x, float y);
	bool shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets) override;
	bool is_a_grenade() override;
	bool in_explosion_area(float x, float y) override;
	bool isShooting() override;
	bool try_to_explode_grenade() override;
	bool is_destroyed() override;
};
#endif

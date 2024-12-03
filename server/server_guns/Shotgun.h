#ifndef SHOTGUN_H
#define SHOTGUN_H

#include "gun.h"
#include "ShotgunAmmo.h"

#include <map>

class Shotgun : public Gun {
private:
	int ammo;
	bool must_recharge;
public:
	Shotgun(float x, float y, std::map<std::string, float> config);
	bool is_duck_position_valid(float x, float y);
	bool shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets) override;
};

#endif

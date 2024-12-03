#ifndef DUELPISTOL_H
#define DUELPISTOL_H

#include <list>
#include <utility>
#include <map>

#include "gun.h"
#include "DuelPistolAmmo.h"

class DuelPistol : public Gun {
private:
	int ammo;
public:
	DuelPistol(float x, float y, std::map<std::string, float> config);
	bool is_duck_position_valid(float x, float y);
	bool shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets) override;
};

#endif

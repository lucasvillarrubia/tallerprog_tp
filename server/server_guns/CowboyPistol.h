#ifndef COWBOYPISTOL_H
#define COWBOYPISTOL_H

#include <list>
#include <utility>

#include "gun.h"
#include "ammo.h"
#include "common/coordinates.h"
#include "CowboyPistolAmmo.h"

class CowboyPistol : public Gun{
private:
	int ammo;
public:
	CowboyPistol(float x, float y);
	bool is_duck_position_valid(float x, float y);
	bool shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets) override;
};

#endif

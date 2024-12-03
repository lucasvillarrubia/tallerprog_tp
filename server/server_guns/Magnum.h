#ifndef MAGNUM_H
#define MAGNUM_H

#include <list>
#include <utility>
#include <map>

#include "gun.h"
#include "common/coordinates.h"
#include "MagnumAmmo.h"
#include "common/rectangulo.h"

class Magnum : public Gun {
private:
	int ammo;
	
public:
	Magnum(float x, float y, std::map<std::string, float> config);
	bool is_duck_position_valid(float x, float y);
	bool shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets) override;
};

#endif


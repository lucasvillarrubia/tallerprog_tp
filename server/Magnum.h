#ifndef MAGNUM_H
#define MAGNUM_H

#include "gun.h"
#include "common/coordinates.h"
#include "MagnumAmmo.h"
#include "common/rectangulo.h"

class Magnum : public Gun {
private:
	int ammo;
	
public:
	Magnum(float x, float y);
	bool is_duck_position_valid(float x, float y);
	MagnumAmmo shoot();
};

#endif


#ifndef MAGNUM_H
#define MAGNUM_H

#include "gun.h"
#include "coordinates.h"
#include "MagnumAmmo.h"

class Magnum : public Gun {
private:
	int ammo;
public:
	Magnum(float x, float y);
	MagnumAmmo shoot();
};

#endif


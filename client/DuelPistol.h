#ifndef DUELPISTOL_H
#define DUELPISTOL_H

#include "gun.h"
#include "coordinates.h"
#include "DuelPistolAmmo.h"

class DuelPistol : public Gun {
private:
	int ammo;
public:
	DuelPistol(float x, float y);
	DuelPistolAmmo shoot();
};

#endif

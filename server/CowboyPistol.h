#ifndef COWBOYPISTOL_H
#define COWBOYPISTOL_H

#include "gun.h"
#include "common/coordinates.h"
#include "CowboyPistolAmmo.h"

class CowboyPistol : public Gun{
private:
	int ammo;
public:
	CowboyPistol(float x, float y);
	CowboyPistolAmmo shoot();
};

#endif

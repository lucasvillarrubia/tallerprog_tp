#ifndef COWBOYPISTOLAMMO_H
#define COWBOYPISTOLAMMO_H

#include <cmath>
#include "ammo.h"
#include "coordinates.h"

class CowboyPistolAmmo : public Ammo {
private:
	float damagePoints;
	float scope;
public:
	CowboyPistolAmmo(float x, float y, bool right);
	CowboyPistolAmmo();
	void updatePosition(const unsigned int frame_delta);
	float impact();
};

#endif

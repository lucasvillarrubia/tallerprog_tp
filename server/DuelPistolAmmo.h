#ifndef DUELPISTOLAMMO_H
#define DUELPISTOLAMMO_H

#include <cmath>
#include "ammo.h"
#include "coordinates.h"

class DuelPistolAmmo : public Ammo {
private:
	float damagePoints;
	float scope;
public:
	DuelPistolAmmo(float x, float y, bool right);
	DuelPistolAmmo();
	void updatePosition(const unsigned int frame_delta);
	float impact();
};

#endif

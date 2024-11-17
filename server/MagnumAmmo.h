#ifndef MAGNUMAMMO_H
#define MAGNUMAMMO_H

#include <cmath>
#include "ammo.h"
#include "common/coordinates.h"

class MagnumAmmo : public Ammo {
private:
	float damagePoints;
	float scope;
public:
	MagnumAmmo(float x, float y, bool right);
	MagnumAmmo();
	void updatePosition(const unsigned int frame_delta);
	float impact();
};

#endif

#ifndef AK47AMMO_H
#define AK47AMMO_H

#include <cmath>
#include "ammo.h"
#include "coordinates.h"

class AK47Ammo : public Ammo {
private:
	float damagePoints;
	float scope;
public:
	AK47Ammo(float x, float y, bool right);
	AK47Ammo();
	void updatePosition(const unsigned int frame_delta);
	float impact();
};

#endif

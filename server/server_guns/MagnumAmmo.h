#ifndef MAGNUMAMMO_H
#define MAGNUMAMMO_H

#include <cmath>
#include "ammo.h"
#include "common/coordinates.h"

class MagnumAmmo : public Ammo {
private:
	float damagePoints;
public:
	MagnumAmmo(float x, float y, bool right);
	void update_position(const unsigned int frame_delta) override;
	float impact() override;
	//~MagnumAmmo() = default;
};

#endif

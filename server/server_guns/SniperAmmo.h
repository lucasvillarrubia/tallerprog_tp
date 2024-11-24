#ifndef SNIPERAMMO_H
#define SNIPERAMMO_H

#include <cmath>
#include "ammo.h"
#include "common/coordinates.h"

class SniperAmmo : public Ammo {
private:
	float damagePoints;
public:
	SniperAmmo(float x, float y, bool right);
	void update_position(const unsigned int frame_delta) override;
	float impact() override;
};

#endif

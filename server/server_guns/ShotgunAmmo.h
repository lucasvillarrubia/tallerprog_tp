#ifndef SHOTGUNAMMO_H
#define SHOTGUNAMMO_H

#include "ammo.h"

class ShotgunAmmo : public Ammo {
private:
	float damagePoints;
public:	
	ShotgunAmmo(float x, float y, bool right, bool up);
	void update_position(const unsigned int frame_delta) override;
	float impact() override;
};

#endif

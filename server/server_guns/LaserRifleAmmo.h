#ifndef LASERRIFLEAMMO_H
#define LASERRIFLEAMMO_H

#include "ammo.h"

class LaserRifleAmmo : public Ammo {
private:
	float damagePoints;
	float scope;
public:
	LaserRifleAmmo(float x, float y, bool right, bool up);
	void update_position(const unsigned int frame_delta) override;
	float impact() override;
};

#endif

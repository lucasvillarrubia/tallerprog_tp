#ifndef PEWPEWLASERAMMO_H
#define PEWPEWLASERAMMO_H

#include "ammo.h"

class PewPewLaserAmmo : public Ammo {
private:
	float scope;
public:
	PewPewLaserAmmo(float x, float y, bool right, bool up);
	void update_position(const unsigned int frame_delta) override;
	float impact() override;
};

#endif

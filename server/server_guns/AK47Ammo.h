#ifndef AK47AMMO_H
#define AK47AMMO_H

#include <cmath>
#include "ammo.h"
#include "common/coordinates.h"

class AK47Ammo : public Ammo {
private:
	float damagePoints;
	float scope;
public:
	AK47Ammo(float x, float y, bool right);
	void update_position(const unsigned int frame_delta) override;
	float impact() override;
};

#endif

#ifndef COWBOYPISTOLAMMO_H
#define COWBOYPISTOLAMMO_H

#include <cmath>
#include "ammo.h"
#include "common/coordinates.h"

class CowboyPistolAmmo : public Ammo {
private:
	float damagePoints;
	float scope;
public:
	CowboyPistolAmmo(float x, float y, bool right);
	void update_position(const unsigned int frame_delta) override;
	bool is_destroyed() override;
	float impact() override;
};

#endif

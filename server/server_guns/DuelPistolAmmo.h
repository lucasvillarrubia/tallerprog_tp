#ifndef DUELPISTOLAMMO_H
#define DUELPISTOLAMMO_H

#include <cmath>
#include "ammo.h"
#include "common/coordinates.h"

class DuelPistolAmmo : public Ammo {
private:
	float damagePoints;
public:
	DuelPistolAmmo(float x, float y, bool right);
	void update_position(const unsigned int frame_delta) override;
	bool is_destroyed() override;
	float impact() override;
};

#endif

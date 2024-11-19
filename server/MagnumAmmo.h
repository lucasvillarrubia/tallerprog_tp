#ifndef MAGNUMAMMO_H
#define MAGNUMAMMO_H

#include <cmath>
#include "ammo.h"
#include "common/coordinates.h"

class MagnumAmmo : public Ammo {
private:
	float damagePoints;
	//float scope;
public:
	MagnumAmmo(float x, float y, bool right);
	//MagnumAmmo();
	void update_position(const unsigned int frame_delta) override;
	bool is_destroyed() override;
	float impact() override;
	~MagnumAmmo() = default;
};

#endif

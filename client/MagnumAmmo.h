#ifndef MAGNUMAMMO_H
#define MAGNUMAMMO_H

#include <cmath>
#include "coordinates.h"

class MagnumAmmo {
private:
	float originX;
	float originY;
	float positionX;
	float positionY;
	float damagePoints;
	float scope;
	bool destroyed;
public:
	MagnumAmmo(float x, float y);
	MagnumAmmo(bool destroy);
	void updatePosition(const unsigned int frame_delta);
	float impact();
	bool isDestroyed();
	Coordinates getPosition();
};

#endif

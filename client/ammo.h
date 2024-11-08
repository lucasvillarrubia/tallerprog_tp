#ifndef AMMO_H
#define AMMO_H

#include "coordinates.h"

class Ammo {
protected:
	float originX;
	float originY;
	float positionX;
	float positionY;
	bool destroyed;
	bool movingRight;
public:
	Ammo(float x, float y, bool right);
	bool isDestroyed();
	Coordinates getPosition();
	bool isMovingRight();
	virtual ~Ammo() = default;
};

#endif

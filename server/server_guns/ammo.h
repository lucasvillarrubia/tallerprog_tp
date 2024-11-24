#ifndef AMMO_H
#define AMMO_H

#include <cmath>
#include "common/coordinates.h"

class Ammo {
protected:
	float originX;
	float originY;
	float positionX;
	float positionY;
	bool destroyed;
	bool movingRight;
	int type_gun;
	float scope;
public:
	Ammo(float x, float y, bool right, int type, float _scope);
	virtual void update_position(unsigned int);
	virtual float impact();
	virtual bool is_destroyed();
	Coordinates getPosition();
	bool isMovingRight();
	int getType();
	virtual ~Ammo() = default;
};

#endif

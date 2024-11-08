#ifndef GUN_H
#define GUN_H

#include "coordinates.h"

class Gun {
protected:
	float positionX;
	float positionY;
	bool shooting;
	bool pickedUp;
	bool rightDirection;
public:
	Gun(float x, float y);
	void updatePosition(float x, float y);
	void updateDirection(bool right);
	Coordinates getPosition();
	void collected();
	bool isShooting();
	bool isPickedUp();
	void stopShoot();
	virtual ~Gun() = default;
};

#endif

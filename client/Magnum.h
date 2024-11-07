#ifndef MAGNUM_H
#define MAGNUM_H

#include "coordinates.h"

class Magnum {
private:
	float positionX;
	float positionY;
	bool shooting;
	bool pickedUp;
	int ammo;
public:
	Magnum(float x, float y);
	void updatePosition(float x, float y);
	Coordinates getPosition();
	float getPositionX();
	float getPositionY();
	void collected();
	bool isShooting();
	bool isPickedUp();
	void shoot();
	void stopShoot();
};

#endif


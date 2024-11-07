#ifndef MAGNUM_H
#define MAGNUM_H

#include "coordinates.h"
#include "MagnumAmmo.h"

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
	void collected();
	bool isShooting();
	bool isPickedUp();
	MagnumAmmo shoot();
	void stopShoot();
};

#endif


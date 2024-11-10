#ifndef AK47_H
#define AK47_H

#include "gun.h"
#include "coordinates.h"
#include "AK47Ammo.h"

#include <chrono>

class AK47 : public Gun {
private:
	int ammo;
	//variables para medir el tiempo entre disparo
public:
	AK47(float x, float y);
	AK47Ammo shoot();
};

#endif

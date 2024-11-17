#ifndef GUN_H
#define GUN_H

#include "common/coordinates.h"
#include "item.h"

class Gun : public Item{
protected:
	bool shooting;
public:
	Gun(float x, float y, float width, float height, int type);
	bool isShooting();
	void stopShoot();
	virtual ~Gun() = default;
};

#endif

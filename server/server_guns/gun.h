#ifndef GUN_H
#define GUN_H

#include <list>
#include <utility>
#include <iostream>

#include "common/coordinates.h"
#include "item.h"
#include "ammo.h"

class Gun : public Item{
protected:
	bool shooting;
public:
	Gun(float x, float y, float width, float height, int type);
	bool is_duck_position_valid(float x, float y);
	bool isShooting();
	virtual bool shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets);
	void stopShoot();
	virtual bool is_banana_peel();
	virtual ~Gun() = default;
};

#endif

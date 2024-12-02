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
	bool pointing_up;
public:
	Gun(float x, float y, float width, float height, int type);
	bool is_duck_position_valid(float x, float y);
	void updateDirection(bool right, bool up);
	bool is_pointing_up();
	virtual bool isShooting();
	virtual bool shoot(int& id, std::list<std::pair<int, Ammo*>>& bullets);
	virtual void stopShoot();
	virtual bool is_banana_peel();
	virtual bool is_a_grenade();
	virtual bool is_pew_pew_laser();
	virtual bool in_explosion_area(float x, float y);
	virtual bool try_to_explode_grenade();
	virtual bool is_destroyed();
	virtual ~Gun() = default;
};

#endif

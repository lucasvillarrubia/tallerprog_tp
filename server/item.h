#ifndef ITEM_H
#define ITEM_H

#include "coordinates.h"

class Item {
protected:	
	float positionX;
	float positionY;
	bool pickedUp;
	bool rightDirection;
public:
	Item(float x, float y);
	void updatePosition(float x, float y);
	void updateDirection(bool right);
	Coordinates getPosition();
	void collected();
	bool isPickedUp();
	virtual ~Item() = default;
};

#endif

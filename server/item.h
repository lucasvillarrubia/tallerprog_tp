#ifndef ITEM_H
#define ITEM_H

#include "common/coordinates.h"
#include "common/rectangulo.h"

class Item {
protected:	
	float positionX;
	float positionY;
	bool pickedUp;
	int itemType;
	bool rightDirection;
	Rectangulo hitbox;
public:
	Item(float x, float y, float width, float height, int type);
	void updatePosition(float x, float y);
	void updateDirection(bool right);
	Coordinates getPosition();
	int getType();
	void collected();
	bool isPickedUp();
	bool is_pointing_to_the_right() const;
	virtual ~Item() = default;
};

#endif

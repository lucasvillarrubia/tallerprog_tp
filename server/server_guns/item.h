#ifndef ITEM_H
#define ITEM_H

#include "common/coordinates.h"
#include "common/rectangulo.h"

class Item {
protected:	
	float positionX;
	float positionY;
	bool pickedUp;
	bool is_dropped;
	int itemType;
	bool rightDirection;
	Rectangulo hitbox;
	bool is_on_the_floor;
	float vertical_velocity;
	//bool out_of_the_map;
public:
	Item(float x, float y, float width, float height, int type);
	virtual void updatePosition(float x, float y);
	void updateDirection(bool right);
	Coordinates getPosition();
	int getType();
	void collected();
	virtual void dropped();
	bool isPickedUp();
	bool is_pointing_to_the_right() const;
	void update_item_dropped_position(const unsigned int frame_delta);
	bool is_on_the_ground();
	void set_is_on_the_floor();
	bool is_falling();
	virtual ~Item() = default;
};

#endif

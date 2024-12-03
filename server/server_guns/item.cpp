#include "item.h"

Item::Item(float x, float y, float width, float height, int type) : positionX(x), positionY(y), pickedUp(false), is_dropped(false), itemType(type), rightDirection(true), hitbox(x, y, width, height), is_on_the_floor(true), vertical_velocity(0.0f) {}

void Item::updatePosition(float x, float y){
	positionX = x;
	positionY = y;
	hitbox.update_position(x,y);
}

void Item::updateDirection(bool right) {
	rightDirection = right;
}

Coordinates Item::getPosition() {
	return { positionX, positionY};
}

int Item::getType() {
	return itemType;
}

void Item::collected() {
	pickedUp = true;
}

void Item::dropped() {
	pickedUp = false;
	is_dropped = true;
	is_on_the_floor = false;
}

bool Item::isPickedUp() {
	return pickedUp;
}

bool Item::is_pointing_to_the_right() const {
	return rightDirection;
}

void Item::update_item_dropped_position(const unsigned int frame_delta) {
	if (is_dropped and !is_on_the_floor) {
		positionX += 32 * (rightDirection ? 1 : -1);
	}
	if (!is_on_the_floor and !pickedUp) {
		positionY += vertical_velocity * (frame_delta * 0.06f);
		vertical_velocity += -0.7f * (frame_delta * 0.06f);
	}
}

bool Item::is_on_the_ground() {
	return is_on_the_floor;
}

void Item::set_is_on_the_floor() {
	is_on_the_floor = true;
	is_dropped = false;
	vertical_velocity = 0;
}

bool Item::is_falling() {
	return is_dropped;
}

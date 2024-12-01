#ifndef GUN_H
#define GUN_H

#include <iostream>

const float GUN_INITIAL_X_COORDINATE = 0.0f;
const float GUN_INITIAL_Y_COORDINATE = 0.0f;

struct Gun {
	int id;
	float pos_X;
	float pos_Y;
	int type;
	bool pointing_to_the_right;
	bool shooting;
	bool is_destroyed;
	bool is_exploding;
	int explosion_phase;
	
	explicit Gun(const int _id) :
		id(_id),
        pos_X(GUN_INITIAL_X_COORDINATE),
        pos_Y(GUN_INITIAL_Y_COORDINATE),
        type(0),
        pointing_to_the_right(true),
        shooting(false),
        is_destroyed(false),
        is_exploding(false),
        explosion_phase(0) {}

	int get_explosion_phase(const unsigned int frame_ticks) const {
		if (is_exploding && !is_destroyed) {
			return (frame_ticks / 6) % 6;
		} else {
			return 0;
		}
		//return explosion_phase;
	}
};

#endif

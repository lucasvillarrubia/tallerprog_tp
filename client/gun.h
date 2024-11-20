#ifndef GUN_H
#define GUN_H

const float GUN_INITIAL_X_COORDINATE = 0.0f;
const float GUN_INITIAL_Y_COORDINATE = 0.0f;

struct Gun {
	int id;
	float pos_X;
	float pos_Y;
	int type;
	bool pointing_to_the_right;
	
	explicit Gun(const int _id) :
		id(_id),
        pos_X(GUN_INITIAL_X_COORDINATE),
        pos_Y(GUN_INITIAL_Y_COORDINATE),
        type(0),
        pointing_to_the_right(true) {}
};

#endif

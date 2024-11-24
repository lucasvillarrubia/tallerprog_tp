#ifndef BULLET_H
#define BULLET_H

const float BULLET_INITIAL_X_COORDINATE = 0.0f;
const float BULLET_INITIAL_Y_COORDINATE = 0.0f;

struct Bullet {
	int id;
	float pos_X;
	float pos_Y;
	int type;
	bool moving_right;
	bool destroyed;
	
	explicit Bullet(const int _id) : 
		id(_id),
		pos_X(BULLET_INITIAL_X_COORDINATE),
		pos_Y(BULLET_INITIAL_Y_COORDINATE),
		type(0),
		moving_right(true),
		destroyed(false) {}
};

#endif

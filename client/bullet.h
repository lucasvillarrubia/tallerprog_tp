#ifndef BULLET_H
#define BULLET_H

const float BULLET_INITIAL_X_COORDINATE = 0.0f;
const float BULLET_INITIAL_Y_COORDINATE = 0.0f;

struct Bullet {
	int id;
	float origin_X;
	float origin_Y;
	float pos_X;
	float pos_Y;
	int type;
	bool moving_right;
	bool moving_up;
	bool destroyed;
	
	explicit Bullet(const int _id) : 
		id(_id),
		origin_X(BULLET_INITIAL_X_COORDINATE),
		origin_Y(BULLET_INITIAL_Y_COORDINATE),
		pos_X(origin_X),
		pos_Y(origin_Y),
		type(0),
		moving_right(true),
		moving_up(false),
		destroyed(false) {}
};

#endif

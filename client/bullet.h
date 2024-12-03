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
	bool moving_up;
	bool destroyed;
	bool reproducir_sonido;
	
	explicit Bullet(const int _id) : 
		id(_id),
		pos_X(BULLET_INITIAL_X_COORDINATE),
		pos_Y(BULLET_INITIAL_Y_COORDINATE),
		type(0),
		moving_right(true),
		moving_up(false),
		destroyed(false),
		reproducir_sonido(true) {}
};

#endif

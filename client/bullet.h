#ifndef BULLET_H
#define BULLET_H

struct Bullet {
	int id;
	int type_gun;
	float pos_X;
	float pos_Y;
	bool destroyed;
	Bullet(int _id, int type, float x, float y) :
		id(_id),
		type_gun(type),
		pos_X(x),
		pos_Y(y),
		destroyed(false) {}
};

#endif

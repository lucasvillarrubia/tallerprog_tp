#ifndef BANANA_H
#define BANANA_H

#include "gun.h"

class Banana : public Gun{
private:
	bool launched;
public:
	Banana(float x, float y);
	void launch();
	bool is_duck_position_valid(float x, float y);
	//asigna velocidad vertical
	//void change_direction(); //en caso de rebotar con una pared
	//void set_trap(); //banana lista para pisarse
	//void make_the_duck_slip() //el pato resbala. determinar cuanto resbala
};

#endif

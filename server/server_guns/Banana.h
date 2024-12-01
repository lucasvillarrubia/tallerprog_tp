#ifndef BANANA_H
#define BANANA_H

#include "gun.h"

class Banana : public Gun{
private:
	bool banana_peel;
public:
	Banana(float x, float y);
	void dropped() override;
	bool is_duck_position_valid(float x, float y);
	bool is_banana_peel() override;
	//asigna velocidad vertical
	//void change_direction(); //en caso de rebotar con una pared
	//void set_trap(); //banana lista para pisarse
	//void make_the_duck_slip() //el pato resbala. determinar cuanto resbala
};

#endif

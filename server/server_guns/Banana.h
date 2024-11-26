#ifndef BANANA_H
#define BANANA_H

#include "item.h"

class Banana : public Item{
private:
	bool launched;
public:
	Banana(float x, float y);
	void launch();
	//asigna velocidad vertical
	//void change_direction(); //en caso de rebotar con una pared
	//void set_trap(); //banana lista para pisarse
	//void make_the_duck_slip() //el pato resbala. determinar cuanto resbala
};

#endif

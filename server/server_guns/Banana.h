#ifndef BANANA_H
#define BANANA_H

#include "item.h"

class Banana : public Item{
private:
	bool launched;
	float vertical_velocity;
public:
	Banana(float x, float y);
	void launch(){}
		//asigna velocidad vertical
	void update_position(){};
		//actualiza posición horizontal de la misma forma que las balas,
		//pero realizando una parabola
};

#endif

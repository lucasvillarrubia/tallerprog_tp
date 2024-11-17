#ifndef ARMOR_H
#define ARMOR_H

#include "common/coordinates.h"
#include "item.h"

class Armor : public Item {
private:
	//hitbox?
public:
	Armor(float x, float y);
	//funcion para recibir daño? probably hacerlo dentro de character
};

#endif

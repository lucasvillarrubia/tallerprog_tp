#include "AK47.h"

AK47::AK47(float x, float y): Gun(x,y), ammo(30) {}

AK47Ammo AK47::shoot() {
	if (!shooting && (ammo > 0) && pickedUp) { //actualmente funciona como arma semiautomatica
		//if (shooting) {
			//logica para tiempo entre disparo
		//}
		int dir = rightDirection ? 36 : -12;
		ammo--;
		return AK47Ammo(positionX+dir, positionY, rightDirection);
	}
	return AK47Ammo();
}


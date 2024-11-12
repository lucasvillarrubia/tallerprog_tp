#ifndef SPAWNPLACE_H
#define SPAWNPLACE_H

#include "Gun.h"
#include "DuelPistol.h"
#include "CowboyPistol.h"
#include "Magnum.h"

class SpawnPlace {
private:
	float positionX;
	float positionY;
	bool itemSpawned;
	bool itemPickedUp;
	//timeGunPickedUp;
	//timeRespawn;
	int idItemToRespawn;
	bool itemToRespawnChoosed;
	
	
public:
	SpawnPlace(float x, float y);
	bool isItemSpawned();
	//void chooseItemToSpawn();
	//Gun spawnGun();
	//DuelPistol spawnDuelPistol();
	//CowboyPistol spawnCowboyPistol();
	//Magnum spawnMagnum();
};

#endif

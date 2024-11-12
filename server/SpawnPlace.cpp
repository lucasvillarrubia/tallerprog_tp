#include "SpawnPlace.h"

SpawnPlace::SpawnPlace(float x, float y) : positionX(x), positionY(y), itemRespawned(false), itemPickedUp, idItemToRespawn(0), itemToRespawnChoosed(false) {
}

bool SpawnPlace::isItemSpawned() {
	return ItemRespawned;
}
/*
void SpawnPlace::chooseItemToSpawn() {
	//idGunToRespawn = random(1,10);
	//setear tiempo de respawneo
	
}

Gun SpawnPlace::spawnGun() {
	switch(idGunToRespawn)
}*/

#ifndef SPAWNPLACE_H
#define SPAWNPLACE_H

#include <map>
#include <chrono>
#include <random>

#include "gun.h"
#include "AK47.h"
#include "DuelPistol.h"
#include "CowboyPistol.h"
#include "Magnum.h"
#include "Sniper.h"

class SpawnPlace {
private:
	const float pos_X;
	const float pos_Y;
	bool item_spawned;
	bool gun_spawned;
	bool item_picked;
	Rectangulo hitbox;
	float min_time_to_respawn;
	std::chrono::time_point<std::chrono::_V2::steady_clock> last_spawn_time;
public:
	SpawnPlace(float x, float y);
	bool is_gun_spawned();
	bool is_item_spawned();
	//bool try_spawn_item(int& id, std::list<Item*>& items);
	bool try_spawn_gun(int& id, std::map<int,Gun*>& guns);
	void spawn_ak47(const int id, std::map<int,Gun*>& guns);
	void spawn_duel_pistol(const int id, std::map<int,Gun*>& guns);
	void spawn_cowboy_pistol(const int id, std::map<int,Gun*>& guns);
	void spawn_magnum(const int id, std::map<int,Gun*>& guns);
	void spawn_sniper(const int id, std::map<int,Gun*>& guns);
	void pick_item();
	bool is_duck_position_valid(float x, float y);
};

#endif

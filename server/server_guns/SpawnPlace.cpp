#include "SpawnPlace.h"

SpawnPlace::SpawnPlace(float x, float y) : pos_X(y), pos_Y(y), item_spawned(false), gun_spawned(false), item_picked(false), hitbox(x,y,32,32), min_time_to_respawn(5) {
	last_spawn_time = std::chrono::steady_clock::now();
}

bool SpawnPlace::is_gun_spawned() {
	return gun_spawned;
}

bool SpawnPlace::is_item_spawned() {
	return item_spawned;
}
/*
bool SpawnPlace::try_spawn_item(int& id, std::map<int, Item*>& items) {
	if (!item_spawned && !gun_spawned) {
		auto time_now = std::chrono::steady_clock::now();
		auto t = std::chrono::duration_cast<std::chrono::second> (time_now-last_spawn_time).count();
		if (t >= min_time_to_respawn) {
			item_picked = false;
			item_spawn = true;
			id++;
			items.insert({id, new item});
			return true;
		}
	}
	return false;
}*/

bool SpawnPlace::try_spawn_gun(int& id, std::map<int, Gun*>& guns) {
	if (!item_spawned && !gun_spawned) {
		auto time_now = std::chrono::steady_clock::now();
		auto t = std::chrono::duration_cast<std::chrono::seconds> (time_now-last_spawn_time).count();
		if (t >= min_time_to_respawn) {
			item_picked = false;
			gun_spawned = true;
			id++;
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> random(5,10);
			int gun_code = random(gen);
			switch (gun_code) {
				case 5:
					spawn_ak47(id, guns);
					break;
				case 6:
					spawn_duel_pistol(id, guns);
					break;
				case 7:
					spawn_cowboy_pistol(id, guns);
					break;
				case 8:
					spawn_magnum(id, guns);
					break;
				default:
					spawn_sniper(id, guns);
					break;
			}
			return true;
		}
	}
	return false;
}

void SpawnPlace::spawn_ak47(const int id, std::map<int,Gun*>& guns) {
	guns.insert({id, new AK47(pos_X,pos_Y)});
}

void SpawnPlace::spawn_duel_pistol(const int id, std::map<int,Gun*>& guns) {
	guns.insert({id, new DuelPistol(pos_X,pos_Y)});
}

void SpawnPlace::spawn_cowboy_pistol(const int id, std::map<int,Gun*>& guns) {
	guns.insert({id, new CowboyPistol(pos_X,pos_Y)});
}

void SpawnPlace::spawn_magnum(const int id, std::map<int,Gun*>& guns) {
	guns.insert({id, new Magnum(pos_X,pos_Y)});
}

void SpawnPlace::spawn_sniper(const int id, std::map<int,Gun*>& guns) {
	guns.insert({id, new Sniper(pos_X,pos_Y)});
}

void SpawnPlace::pick_item() {
	item_picked = true;
	last_spawn_time = std::chrono::steady_clock::now();
	item_spawned = false;
	gun_spawned = false;
}

bool SpawnPlace::is_duck_position_valid(float x, float y) {
	Rectangulo character(x, y, 64.0f, 64.0f);
    return hitbox.hay_colision(character);
}

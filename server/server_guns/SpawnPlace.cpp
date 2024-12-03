#include "SpawnPlace.h"

SpawnPlace::SpawnPlace(float x, float y, ConfigGuns& config_guns) : config(config_guns), pos_X(x), pos_Y(y), item_spawned(false), gun_spawned(false), item_picked(false), hitbox(x,y,32,32), min_time_to_respawn(5) {
	last_spawn_time = std::chrono::steady_clock::now();
}

bool SpawnPlace::is_gun_spawned() {
	return gun_spawned;
}

bool SpawnPlace::is_item_spawned() {
	return item_spawned;
}


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
			std::uniform_int_distribution<> random(1,10);
			int gun_code = random(gen);
			switch (gun_code) {
				case 1:
					spawn_grenade(id, guns);
					break;
				case 2:
					spawn_banana(id, guns);
					break;
				case 3:
					spawn_pew_pew_laser(id, guns);
					break;
				case 4:
					spawn_laser_rifle(id, guns);
					break;
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
				case 9:
					spawn_shotgun(id, guns);
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

void SpawnPlace::spawn_grenade(const int id, std::map<int,Gun*>& guns) {
	guns.insert({id, new Grenade(pos_X,pos_Y,config.get_grenade_settings())});
}

void SpawnPlace::spawn_banana(const int id, std::map<int,Gun*>& guns) {
	guns.insert({id, new Banana(pos_X,pos_Y,config.get_banana_settings())});
}

void SpawnPlace::spawn_ak47(const int id, std::map<int,Gun*>& guns) {
	guns.insert({id, new AK47(pos_X,pos_Y,config.get_ak47_settings())});
}

void SpawnPlace::spawn_pew_pew_laser(const int id, std::map<int,Gun*>& guns) {
	guns.insert({id, new PewPewLaser(pos_X,pos_Y,config.get_pew_pew_laser_settings())});
}

void SpawnPlace::spawn_laser_rifle(const int id, std::map<int,Gun*>& guns) {
	guns.insert({id, new LaserRifle(pos_X,pos_Y,config.get_laser_rifle_settings())});
}

void SpawnPlace::spawn_duel_pistol(const int id, std::map<int,Gun*>& guns) {
	guns.insert({id, new DuelPistol(pos_X,pos_Y,config.get_duel_pistol_settings())});
}

void SpawnPlace::spawn_cowboy_pistol(const int id, std::map<int,Gun*>& guns) {
	guns.insert({id, new CowboyPistol(pos_X,pos_Y,config.get_cowboy_pistol_settings())});
}

void SpawnPlace::spawn_magnum(const int id, std::map<int,Gun*>& guns) {
	guns.insert({id, new Magnum(pos_X,pos_Y,config.get_magnum_settings())});
}

void SpawnPlace::spawn_shotgun(const int id, std::map<int,Gun*>& guns) {
	guns.insert({id, new Shotgun(pos_X,pos_Y,config.get_shotgun_settings())});
}

void SpawnPlace::spawn_sniper(const int id, std::map<int,Gun*>& guns) {
	guns.insert({id, new Sniper(pos_X,pos_Y,config.get_sniper_settings())});
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

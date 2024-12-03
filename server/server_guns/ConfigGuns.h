#ifndef CONFIGGUNS_H
#define CONFIGGUNS_H

#include <yaml-cpp/yaml.h>
#include <map>
#include <iostream>

class ConfigGuns {
private:
	YAML::Node config;
	std::map<std::string,float> banana_settings;
	std::map<std::string,float> grenade_settings;
	std::map<std::string,float> pew_pew_laser_settings;
	std::map<std::string,float> laser_rifle_settings;
	std::map<std::string,float> ak47_settings;
	std::map<std::string,float> duel_pistol_settings;
	std::map<std::string,float> cowboy_pistol_settings;
	std::map<std::string,float> magnum_settings;
	std::map<std::string,float> shotgun_settings;
	std::map<std::string,float> sniper_settings;
public:
	ConfigGuns() : config(YAML::LoadFile("resources/config_guns.yaml")){
		if (config["BANANA"]) {
			for (const auto& banana : config["BANANA"]){
			banana_settings["DIMENSION_X"] = banana["DIMENSION_X"].as<int>();
			banana_settings["DIMENSION_Y"] = banana["DIMENSION_Y"].as<float>();
			banana_settings["TYPE"] = banana["TYPE"].as<float>();
			}
		}
		if (config["GRENADE"]) {
			for (const auto& grenade : config["GRENADE"]){
			grenade_settings["DIMENSION_X"] = grenade["DIMENSION_X"].as<float>();
			grenade_settings["DIMENSION_Y"] = grenade["DIMENSION_Y"].as<float>();
			grenade_settings["TYPE"] = grenade["TYPE"].as<float>();
			grenade_settings["EXPLOSION_RADIUS"] = grenade["EXPLOSION_RADIUS"].as<float>();
			grenade_settings["DETONATION_TIME"] = grenade["DETONATION_TIME"].as<float>();
			}
		}
		if (config["PEW_PEW_LASER"]) {
			for (const auto& pew_pew_laser : config["PEW_PEW_LASER"]){
			pew_pew_laser_settings["DIMENSION_X"] = pew_pew_laser["DIMENSION_X"].as<float>();
			pew_pew_laser_settings["DIMENSION_Y"] = pew_pew_laser["DIMENSION_Y"].as<float>();
			pew_pew_laser_settings["TYPE"] = pew_pew_laser["TYPE"].as<float>();
			pew_pew_laser_settings["AMMO"] = pew_pew_laser["AMMO"].as<float>();
			pew_pew_laser_settings["TIME_BETWEEN_SHOTS"] = pew_pew_laser["TIME_BETWEEN_SHOTS"].as<float>();
			pew_pew_laser_settings["BURST_SHOTS"] = pew_pew_laser["BURST_SHOTS"].as<float>();
			pew_pew_laser_settings["SCOPE"] = pew_pew_laser["SCOPE"].as<float>();
			}
		}
		if (config["RIFLE_LASER"]) {
			for (const auto& laser_rifle : config["RIFLE_LASER"]){
			laser_rifle_settings["DIMENSION_X"] = laser_rifle["DIMENSION_X"].as<float>();
			laser_rifle_settings["DIMENSION_Y"] = laser_rifle["DIMENSION_Y"].as<float>();
			laser_rifle_settings["TYPE"] = laser_rifle["TYPE"].as<float>();
			laser_rifle_settings["AMMO"] = laser_rifle["AMMO"].as<float>();
			laser_rifle_settings["TIME_BETWEEN_SHOTS"] = laser_rifle["TIME_BETWEEN_SHOTS"].as<float>();
			laser_rifle_settings["SCOPE"] = laser_rifle["SCOPE"].as<float>();
			}
		}
		if (config["AK47"]) {
			for (const auto& ak47 : config["AK47"]){
			ak47_settings["DIMENSION_X"] = ak47["DIMENSION_X"].as<float>();
			ak47_settings["DIMENSION_Y"] = ak47["DIMENSION_Y"].as<float>();
			ak47_settings["TYPE"] = ak47["TYPE"].as<float>();
			ak47_settings["AMMO"] = ak47["AMMO"].as<float>();
			ak47_settings["TIME_BETWEEN_SHOTS"] = ak47["TIME_BETWEEN_SHOTS"].as<float>();
			ak47_settings["SCOPE"] = ak47["SCOPE"].as<float>();
			}
		}
		if (config["DUEL_PISTOL"]) {
			for (const auto& duel_pistol : config["DUEL_PISTOL"]){
			duel_pistol_settings["DIMENSION_X"] = duel_pistol["DIMENSION_X"].as<float>();
			duel_pistol_settings["DIMENSION_Y"] = duel_pistol["DIMENSION_Y"].as<float>();
			duel_pistol_settings["TYPE"] = duel_pistol["TYPE"].as<float>();
			duel_pistol_settings["AMMO"] = duel_pistol["AMMO"].as<float>();
			duel_pistol_settings["SCOPE"] = duel_pistol["SCOPE"].as<float>();
			}
		}
		if (config["COWBOY_PISTOL"]) {
			for (const auto& cowboy_pistol : config["COWBOY_PISTOL"]){
			cowboy_pistol_settings["DIMENSION_X"] = cowboy_pistol["DIMENSION_X"].as<float>();
			cowboy_pistol_settings["DIMENSION_Y"] = cowboy_pistol["DIMENSION_Y"].as<float>();
			cowboy_pistol_settings["TYPE"] = cowboy_pistol["TYPE"].as<float>();
			cowboy_pistol_settings["AMMO"] = cowboy_pistol["AMMO"].as<float>();
			cowboy_pistol_settings["SCOPE"] = cowboy_pistol["SCOPE"].as<float>();
			}
		}
		if (config["MAGNUM"]) {
			for (const auto& magnum : config["MAGNUM"]){
			magnum_settings["DIMENSION_X"] = magnum["DIMENSION_X"].as<float>();
			magnum_settings["DIMENSION_Y"] = magnum["DIMENSION_Y"].as<float>();
			magnum_settings["TYPE"] = magnum["TYPE"].as<float>();
			magnum_settings["AMMO"] = magnum["AMMO"].as<float>();
			magnum_settings["SCOPE"] = magnum["SCOPE"].as<float>();
			}
		}
		if (config["SHOTGUN"]) {
			for (const auto& shotgun : config["SHOTGUN"]){
			shotgun_settings["DIMENSION_X"] = shotgun["DIMENSION_X"].as<float>();
			shotgun_settings["DIMENSION_Y"] = shotgun["DIMENSION_Y"].as<float>();
			shotgun_settings["TYPE"] = shotgun["TYPE"].as<float>();
			shotgun_settings["AMMO"] = shotgun["AMMO"].as<float>();
			shotgun_settings["SCOPE"] = shotgun["SCOPE"].as<float>();
			}
		}
		if (config["SNIPER"]) {
			for (const auto& sniper : config["SNIPER"]){
			sniper_settings["DIMENSION_X"] = sniper["DIMENSION_X"].as<float>();
			sniper_settings["DIMENSION_Y"] = sniper["DIMENSION_Y"].as<float>();
			sniper_settings["TYPE"] = sniper["TYPE"].as<float>();
			sniper_settings["AMMO"] = sniper["AMMO"].as<float>();
			sniper_settings["TIME_BETWEEN_SHOTS"] = sniper["TIME_BETWEEN_SHOTS"].as<float>();
			sniper_settings["SCOPE"] = sniper["SCOPE"].as<float>();
			}
		}
	}
	
	std::map<std::string,float> get_banana_settings() {
		return banana_settings;
	}
	
	std::map<std::string,float> get_grenade_settings() {
		return grenade_settings;
	}
	
	std::map<std::string,float> get_pew_pew_laser_settings() {
		return pew_pew_laser_settings;
	}
	
	std::map<std::string,float> get_laser_rifle_settings() {
		return laser_rifle_settings;
	}
	
	std::map<std::string,float> get_ak47_settings() {
		return ak47_settings;
	}
	
	std::map<std::string,float> get_duel_pistol_settings() {
		return duel_pistol_settings;
	}
	
	std::map<std::string,float> get_cowboy_pistol_settings() {
		return cowboy_pistol_settings;
	}
	
	std::map<std::string,float> get_magnum_settings() {
		return magnum_settings;
	}
	
	std::map<std::string,float> get_shotgun_settings() {
		return shotgun_settings;
	}
	
	std::map<std::string,float> get_sniper_settings() {
		return sniper_settings;
	}
};

#endif

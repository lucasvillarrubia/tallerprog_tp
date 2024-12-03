#ifndef TERRAIN_H
#define TERRAIN_H
#include <list>
#include <map>
#include <fstream>
#include "common/rectangulo.h"
#include <common/coordinates.h>
#include "duck.h"
#include "server_guns/gun.h"
#include "server/server_guns/SpawnPlace.h"
#include "server/server_guns/ConfigGuns.h"
#include <yaml-cpp/yaml.h>

class Terrain {
private:
    YAML::Node config;
    std::map<int, Rectangulo> map_entities_by_id;
    std::list<Rectangulo> map_entities;
    std::list<SpawnPlace>& spawn_places;
    std::map<int, Gun*>& guns_by_id;
    std::map<int, Duck>& ducks_by_id;
    ConfigGuns config_guns;
public:
    Terrain(std::list<SpawnPlace>&, std::map<int, Gun*>&, std::map<int, Duck>&);
    void set_ducks_positions();
    bool is_duck_position_valid(int, int);
    void adjust_position_for_collisions(Duck&, const Coordinates&, const Coordinates&);
    void adjust_position_for_collisions(Gun*, const Coordinates&, const Coordinates&);
    //void adjust_position_for_collisions(Duck&, const Coordinates&, const Coordinates&);

    std::string getCurrentMap() {
        std::ifstream mapFile("resources/current_map.yaml");
        YAML::Node config = YAML::Load(mapFile);
        
        std::string mapName = config["current_map"].as<std::string>();
        return "resources/" + mapName + ".yaml";
    }

    ~Terrain() = default;

};


#endif

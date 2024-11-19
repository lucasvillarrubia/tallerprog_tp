#ifndef TERRAIN_H
#define TERRAIN_H
#include <list>
#include <map>
#include <fstream>
#include "common/rectangulo.h"
#include <common/coordinates.h>
#include "duck.h"
#include <yaml-cpp/yaml.h>

class Terrain {
private:
    std::map<int, Rectangulo> map_entities_by_id;
    std::list<Rectangulo> map_entities;
public:
    Terrain();
    bool is_duck_position_valid(int, int);
    void adjust_position_for_collisions(Duck&, const Coordinates&, const Coordinates&);

    std::string getCurrentMap() {
        std::ifstream mapFile("resources/current_map.yaml");
        YAML::Node config = YAML::Load(mapFile);
        
        std::string mapName = config["current_map"].as<std::string>();
        return "resources/" + mapName + ".yaml";
    }

    ~Terrain() = default;

};


#endif

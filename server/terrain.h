#ifndef TERRAIN_H
#define TERRAIN_H
#include <list>

#include "common/map_entity.h"


class Terrain {
private:
    std::list<MapEntity> map_entities;
public:
    Terrain();
    bool is_duck_position_valid(int, int);
    ~Terrain() = default;
};



#endif

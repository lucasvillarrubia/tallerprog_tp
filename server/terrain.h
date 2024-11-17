#ifndef TERRAIN_H
#define TERRAIN_H
#include <list>
#include <map>

#include "common/rectangulo.h"
#include <common/coordinates.h>
#include "duck.h"


class Terrain {
private:
    std::map<int, Rectangulo> map_entities_by_id;
    std::list<Rectangulo> map_entities;
public:
    Terrain();
    bool is_duck_position_valid(int, int);
    void adjust_position_for_collisions(Duck&, const Coordinates&, const Coordinates&);
    ~Terrain() = default;
};


#endif

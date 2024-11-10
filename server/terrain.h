#ifndef TERRAIN_H
#define TERRAIN_H
#include <list>

#include "common/rectangulo.h"


class Terrain {
private:
    std::list<Rectangulo> map_entities;
public:
    Terrain();
    bool is_duck_position_valid(int, int);
    ~Terrain() = default;
};



#endif

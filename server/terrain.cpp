#include "terrain.h"

bool Terrain::is_duck_position_valid(int x, int y)
{
    MapEntity character(x, y, 64, 64);
    for (auto& entity : map_entities)
    {
        if(entity.hay_colision(character))
            return false;
    }
    return true;
}

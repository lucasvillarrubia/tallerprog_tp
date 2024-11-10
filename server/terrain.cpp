#include "terrain.h"

Terrain::Terrain() {
    map_entities.push_back(Rectangulo(120, 64, 300, 100));
}

bool Terrain::is_duck_position_valid(int x, int y)
{
    Rectangulo character(x, y, 64, 64);
    for (auto& entity : map_entities)
    {
        if(entity.hay_colision(character))
        // if (entity.hay_colision_y(character) and not entity.hay_colision_x(character))
            return false;
    }
    return true;
}

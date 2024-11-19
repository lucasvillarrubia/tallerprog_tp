#include "terrain.h"

Terrain::Terrain() {
    map_entities.push_back(Rectangulo(120.0f, 0.0f, 400.0f, 50.0f));
    map_entities.push_back(Rectangulo(0.0f, 150.0f, 100.0f, 50.0f));
    map_entities.push_back(Rectangulo(540.0f, 150.0f, 100.0f, 50.0f));
}

bool Terrain::is_duck_position_valid(int x, int y)
{
    Rectangulo character(x, y, 64.0f, 64.0f);
    for (auto& entity : map_entities)
    {
        if(entity.hay_colision(character))
        // if (entity.hay_colision_y(character) and not entity.hay_colision_x(character))
            return false;
    }
    return true;
}

bool Terrain::is_bullet_position_valid(int x, int y)
{
    Rectangulo bullet(x, y, 16.0f, 16.0f);
    for (auto& entity : map_entities)
    {
        if(entity.hay_colision(bullet))
        // if (entity.hay_colision_y(character) and not entity.hay_colision_x(character))
            return false;
    }
    return true;
}

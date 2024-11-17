#include "terrain.h"
#include <iostream>

Terrain::Terrain() {
    map_entities.push_back(Rectangulo(120.0f, 0.0f, 400.0f, 50.0f));
    map_entities.push_back(Rectangulo(0.0f, 150.0f, 100.0f, 50.0f));
    map_entities.push_back(Rectangulo(540.0f, 150.0f, 100.0f, 50.0f));
}
// mapas de distintas rondas iguales
// 2 para demo
// posibilidad de agregar mapas desde un archivo

// como crear escenarios propios: agregar a documentación
// aunque no sea con el editor
bool Terrain::is_duck_position_valid(int x, int y)
{
    Rectangulo character(x, y, 64.0f, 64.0f);
    for (auto& entity : map_entities)
    {
        if(entity.hay_colision(character))
            return false;
    }
    return true;
}

void Terrain::adjust_position_for_collisions(Duck& duck, const Coordinates& position)
{
    Rectangulo character(position.pos_X, position.pos_Y, 64.0f, 64.0f);
    bool is_falling = true;
    for (auto& entity : map_entities)
    {
        Coordinates adjusted = position;
        if(entity.hay_colision(character)) {
            if (duck.is_on_the_ground()) {
                adjusted.pos_Y = entity.get_ground_level();
                is_falling = false;
            } else if(duck.is_falling()) {
                adjusted.pos_Y = entity.get_ground_level();
                duck.set_is_on_the_floor();
                is_falling = false;
            }
            else if (duck.is_walking() and duck.is_moving_to_the_right()) 
                adjusted.pos_X = entity.get_left_side() - 64.0f;
            else if (duck.is_walking() and not duck.is_moving_to_the_right()) 
                adjusted.pos_X = entity.get_right_side();
        }
        duck.set_position(adjusted);
    }
    if (is_falling) {
        duck.set_is_NOT_on_the_floor();
    }
    // print is_falling
    // std::cout << "is_falling: " << is_falling << "\n";
}


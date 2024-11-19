#include "terrain.h"
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <vector>
#include <fstream>

Terrain::Terrain() {
    // Leer el archivo YAML
    YAML::Node config = YAML::LoadFile("resources/mapa_azul.yaml");

    // Verificar si 'entities' está en el archivo
    if (config["entities"]) {
        // Recorrer las entidades en el archivo YAML
        for (const auto& entity : config["entities"]) {
            float x = entity["x"].as<float>();
            float y = entity["y"].as<float>();
            float width = entity["width"].as<float>();
            float height = entity["height"].as<float>();

            // Crear un nuevo Rectangulo y agregarlo a la lista
            map_entities.push_back(Rectangulo(x, y, width, height));
        }
    } else {
        std::cerr << "No se encontró la sección 'entities' en el archivo YAML." << std::endl;
    }
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

void Terrain::adjust_position_for_collisions(Duck& duck, const Coordinates& position_before, const Coordinates& position_after)
{
    Rectangulo character(position_after.pos_X, position_after.pos_Y, 64.0f, 64.0f);
    Rectangulo character_before(position_before.pos_X, position_before.pos_Y, 64.0f, 64.0f);
    bool is_falling = true;
    bool can_move_to_the_right = true;
    bool can_move_to_the_left = true;
    Coordinates adjusted = position_after;
    for (auto& entity : map_entities)
    {
        if(entity.hay_colision(character)) {
            if (duck.is_on_the_ground()) {
                is_falling = false;
            }
            if(duck.is_falling() and not entity.hay_colision_y(character_before) and entity.hay_colision_x(character_before)) {
                adjusted.pos_Y = entity.get_ground_level();
                duck.set_is_on_the_floor();
                is_falling = false;
                duck.set_position(adjusted);
            } else if (duck.is_falling() and entity.hay_colision_y(character_before) and not entity.hay_colision_x(character_before)) {
                duck.set_position(position_before.pos_X, position_after.pos_Y);
                if (entity.get_ground_level() <= position_after.pos_Y) {
                    duck.set_is_on_the_floor();
                    is_falling = false;
                }
            } else if (not duck.is_falling() and entity.hay_colision_y(character_before) and not entity.hay_colision_x(character_before)) {
                duck.set_position(position_before);
                duck.stop_flying();
            } else if (not duck.is_falling() and entity.hay_colision_y(character_before) and entity.hay_colision_x(character_before)) {
                duck.set_position(position_after.pos_X, position_before.pos_Y);
                duck.stop_flying();
                if (entity.get_ground_level() <= position_after.pos_Y) {
                    duck.set_is_on_the_floor();
                    is_falling = false;
                }
            }
            else {
                duck.set_position(position_before.pos_X, position_after.pos_Y);
                if (entity.get_ground_level() <= position_after.pos_Y) {
                    duck.set_is_on_the_floor();
                    is_falling = false;
                }
            }
        }
    }
    if (is_falling)
        duck.set_is_NOT_on_the_floor();
    if (can_move_to_the_right)
        duck.set_is_NOT_stomping_on_wall_from_left();
    if (can_move_to_the_left)
        duck.set_is_NOT_stomping_on_wall_from_right();
}


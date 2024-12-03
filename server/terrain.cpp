#include "terrain.h"
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <vector>
#include <fstream>

Terrain::Terrain(std::list<SpawnPlace>& spawn_places, std::map<int, Gun*>& guns_by_id, std::map<int, Duck>& ducks_by_id):
        config(YAML::LoadFile(getCurrentMap())), spawn_places(spawn_places), guns_by_id(guns_by_id), ducks_by_id(ducks_by_id)
{    
    // YAML::Node config = YAML::LoadFile(getCurrentMap());

    if (config["entities"]) {
        for (const auto& entity : config["entities"]) {
            float x = entity["x"].as<float>();
            float y = entity["y"].as<float>();
            float width = entity["width"].as<float>();
            float height = entity["height"].as<float>();

            map_entities.push_back(Rectangulo(x, y, width, height));
        }
    } else {
        std::cerr << "No se encontró la sección 'entities' en el archivo YAML." << std::endl;
    }
    set_spawn_places();
}

void Terrain::set_ducks_positions()
{
    if (config["spawn_places_patos"]) {
        int i = 0;
        for (auto& [id, duck] : ducks_by_id) {
            const auto& entity = config["spawn_places_patos"][i];
            float x = entity["x"].as<float>();
            float y = entity["y"].as<float>();
            duck.set_position(x, y);
            duck.set_is_on_the_floor();
            i++;
        }
    } else {
        std::cerr << "No se encontró la sección 'spawn_places_patos' en el archivo YAML." << std::endl;
    }
}

void Terrain::set_spawn_places()
{
    if (config["spawn_places_armas"]) {
        for (const auto& entity : config["spawn_places_armas"]) {
            float x = entity["x"].as<float>();
            float y = entity["y"].as<float>();
            spawn_places.push_back(SpawnPlace(x, y, config_guns));
        }
    } else {
        std::cerr << "No se encontró la sección 'spawn_places_armas' en el archivo YAML." << std::endl;
    }
}

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

void Terrain::adjust_position_for_collisions(Gun* gun, const Coordinates& position_before, const Coordinates& position_after)
{
    Rectangulo character(position_after.pos_X, position_after.pos_Y, 64.0f, 64.0f);
    Rectangulo character_before(position_before.pos_X, position_before.pos_Y, 64.0f, 64.0f);
    //bool is_falling = true;
    //bool can_move_to_the_right = true;
    //bool can_move_to_the_left = true;
    Coordinates adjusted = position_after;
    for (auto& entity : map_entities)
    {
        if(entity.hay_colision(character)) {
            if (gun->is_on_the_ground()) {
                //is_falling = false;
            }
            if(gun->is_falling() and not entity.hay_colision_y(character_before) and entity.hay_colision_x(character_before)) {
                adjusted.pos_Y = entity.get_ground_level();
                gun->set_is_on_the_floor();
                //is_falling = false;
                gun->updatePosition(adjusted.pos_X,adjusted.pos_Y);
            } else if (gun->is_falling() and entity.hay_colision_y(character_before) and not entity.hay_colision_x(character_before)) {
                gun->updatePosition(position_before.pos_X, position_after.pos_Y);
                if (entity.get_ground_level() <= position_after.pos_Y) {
                    gun->set_is_on_the_floor();
                    //is_falling = false;
                }
            } else if (not gun->is_falling() and entity.hay_colision_y(character_before) and not entity.hay_colision_x(character_before)) {
                gun->updatePosition(position_before.pos_X,position_before.pos_Y);
                //duck.stop_flying();
            } else if (not gun->is_falling() and entity.hay_colision_y(character_before) and entity.hay_colision_x(character_before)) {
                gun->updatePosition(position_after.pos_X, position_before.pos_Y);
                //duck.stop_flying();
                if (entity.get_ground_level() <= position_after.pos_Y) {
                    gun->set_is_on_the_floor();
                    //is_falling = false;
                }
            }
            else {
                gun->updatePosition(position_before.pos_X, position_after.pos_Y);
                if (entity.get_ground_level() <= position_after.pos_Y) {
                    gun->set_is_on_the_floor();
                    //is_falling = false;
                }
            }
        }
    }
}

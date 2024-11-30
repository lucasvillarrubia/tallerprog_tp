#ifndef RENDERER_H
#define RENDERER_H

#include <fstream>
#include "client/character.h"
#include "common/gamedata.h"
#include "common/hands_on_threads/queue.h"
#include "common/hands_on_threads/thread.h"
#include "SDL2pp/Renderer.hh"
#include "SDL2pp/Window.hh"
#include "state_manager.h"
#include "texture_manager.h"
#include <yaml-cpp/yaml.h>
class Renderer
{
private:
    
    std::atomic_bool& connected;
    SDL2pp::Window window;
    // SDL2pp::Window& window;
    SDL2pp::Renderer renderer;
    // SDL2pp::Renderer& renderer;
    Queue<Gamestate>& updates_feed;
    StateManager& state;
    TextureManager textureManager;
    float zoom_factor;
    
    
public:
    Renderer(std::atomic_bool&, Queue<Gamestate>&, StateManager&);
    
    void open_window() {
        window.Show();
    }
    void close_window() {
        window.Hide();
    }



    float get_zoom_factor() const { return zoom_factor; }
    void set_zoom_factor(float new_zoom_factor) { zoom_factor = new_zoom_factor; }

    void draw_bg();
    void draw_character(Character&, int, const float zoom_offset_x, const float zoom_offset_y);
    void draw_gun(Gun& gun, const float zoom_offset_x, const float zoom_offset_y);
    void draw_bullet(Bullet& bullet,  const float zoom_offset_x, const float zoom_offset_y);
    SDL_Rect search_sprite(const int type);
    SDL_Rect search_dimension_sprite(int vcenter, Gun& gun, const float zoom_offset_x, const float zoom_offset_y);
    void render(int);
    void calculate_zoom_offsets(float& offset_x, float& offset_y, float avg_x, float avg_y);
    void dibujar_mapa(const float zoom_offset_x, const float zoom_offset_y);
    void calculate_required_zoom(const std::vector<Coordinates>& duck_positions);

    std::string getCurrentMap() {
        std::ifstream mapFile("resources/current_map.yaml");
        YAML::Node config = YAML::Load(mapFile);
        
        std::string mapName = config["current_map"].as<std::string>();
        return "resources/" + mapName + ".yaml";
    }

    std::string get_fondo();
    ~Renderer() = default;
};


#endif

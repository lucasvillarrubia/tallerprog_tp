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
    struct ZoomState {
        float min_x = 0.0f, max_x = 0.0f;
        float min_y = 0.0f, max_y = 0.0f;
        float target_zoom = 1.0f;
        float current_zoom = 1.0f;
    } zoom_state;
    static constexpr float MIN_ZOOM = 0.1f;
    static constexpr float MAX_ZOOM = 1.0f;
    static constexpr float ZOOM_SMOOTH = 0.1f;
    static constexpr float PADDING = 300.0f;
    std::atomic_bool& connected;
    SDL2pp::Window window;
    // SDL2pp::Window& window;
    SDL2pp::Renderer renderer;
    // SDL2pp::Renderer& renderer;
    Queue<Gamestate>& updates_feed;
    StateManager& state;
    TextureManager textureManager;
    float zoom_factor;
    YAML::Node config_bg;
    YAML::Node config_map;
    const int& player_count;
    std::vector<Coordinates> duck_positions;
    Gamestate current_state;
    std::list<Character> character_list;
    std::list<Gun> gun_list;
    std::list<Bullet> bullet_list;
    std::list<Explosion> explosion_list;
    int original_window_width;
    int original_window_height;
    std::unique_ptr<SDL2pp::Texture> map_texture;
    SDL2pp::Texture* bg_texture;
    void calculate_zoom_offsets(float& offset_x, float& offset_y, float avg_x, float avg_y);
    void dibujar_cajas(const float zoom_offset_x, const float zoom_offset_y);
    void dibujar_mapa(const float zoom_offset_x, const float zoom_offset_y);
    void calculate_required_zoom();
    std::string getCurrentMap() {
        std::ifstream mapFile("resources/current_map.yaml");
        YAML::Node config = YAML::Load(mapFile);
        std::string mapName = config["current_map"].as<std::string>();
        return "resources/" + mapName + ".yaml";
    }
    void playJumpSound(const std::string& soundName, int volume);
    void draw_bg();
    void draw_character(Character&, int, const float zoom_offset_x, const float zoom_offset_y);
    void draw_gun(Gun& gun, const float zoom_offset_x, const float zoom_offset_y);
    void draw_bullet(Bullet& bullet, const float zoom_offset_x, const float zoom_offset_y);
    void draw_explosion(Explosion& explosion, const float zoom_offset_x, const float zoom_offset_y);
    SDL_Rect search_sprite(Gun& gun);
    SDL_Rect search_dimension_sprite(int vcenter, Gun& gun, const float zoom_offset_x, const float zoom_offset_y);
    std::string get_fondo();
public:
    Renderer(std::atomic_bool&, Queue<Gamestate>&, StateManager&, const int&);
    void open_window() { window.Show(); }
    void close_window() { window.Hide(); }
    float get_zoom_factor() const { return zoom_factor; }
    void set_zoom_factor(float new_zoom_factor) { zoom_factor = new_zoom_factor; }
    void render(int);
    void reserve_for_players() {
        duck_positions.clear();
        duck_positions.shrink_to_fit();
        duck_positions.reserve(player_count); 
    }
    ~Renderer() = default;
};


#endif

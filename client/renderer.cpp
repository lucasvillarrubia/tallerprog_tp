#include "renderer.h"

#include <list>
#include <SDL.h>

#include "client/character.h"
#include "client/state_manager.h"
#include "SDL2pp/Renderer.hh"
#include "SDL2pp/SDL.hh"
#include "SDL2pp/Surface.hh"
#include "SDL2pp/Texture.hh"
#include "SDL2pp/Window.hh"
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <vector>
#include <fstream>

const int DUCK_SPRITE_WIDTH = 64;
const int DUCK_SPRITE_HEIGHT = 64;
const int DUCK_MOVEMENT_SPRITES_LINE = 0;


Renderer::Renderer(std::atomic_bool& con_stat, SDL2pp::Window& w, SDL2pp::Renderer& r, Queue<Gamestate>& q, StateManager& s): connected(con_stat), window(w), renderer(r), updates_feed(q), state(s), textureManager(r) {}


void Renderer::draw_character(SDL2pp::Texture& sprites, Character& character, int frame, const float zoom_offset_x, const float zoom_offset_y)
{
    int vcenter = renderer.GetOutputHeight();
    int src_x = DUCK_SPRITE_WIDTH * character.get_movement_phase(frame);
    int src_y = DUCK_MOVEMENT_SPRITES_LINE;
    SDL_RendererFlip flip = character.moving_right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_Rect src_rect = { src_x, src_y, DUCK_SPRITE_WIDTH, DUCK_SPRITE_HEIGHT };

    SDL_Rect dst_rect = { 
        static_cast<int>(character.pos_X + zoom_offset_x), 
        static_cast<int>(vcenter - DUCK_SPRITE_HEIGHT - character.pos_Y + zoom_offset_y), 
        DUCK_SPRITE_WIDTH, 
        DUCK_SPRITE_HEIGHT 
    };

    SDL_RenderCopyEx(renderer.Get(), sprites.Get(), &src_rect, &dst_rect, 0.0, nullptr, flip);
}
// renderizado de mapa con cámara:
// - mapa en textura completa con foco en una parte del mapa
// rectángulo que engloba a los patos
// solo se renderiza rectángulo con patos

void Renderer::calculate_zoom_offsets(float& offset_x, float& offset_y, float avg_x, float avg_y) {
    int window_width = window.GetWidth();
    int window_height = window.GetHeight();
    
    // Center points should be at window_width/2 and window_height/2
    float target_center_x = window_width / 2.0f;
    float target_center_y = window_height * 0.7f;  // Move the center point lower (70% down the screen)
    
    // Calculate how much we need to shift to center both X and Y positions
    float center_offset_x = target_center_x - avg_x;
    
    // Invert the Y offset calculation since SDL Y coordinates increase downward
    float center_offset_y = (target_center_y - (window_height - avg_y));
    
    // Calculate zoom-related offsets
    float zoomed_width = window_width * zoom_factor;
    float zoomed_height = window_height * zoom_factor;
    float width_diff = zoomed_width - window_width;
    float height_diff = zoomed_height - window_height;
    
    // Combine centering offsets with zoom offsets
    offset_x = center_offset_x + (width_diff / -2.0f);
    offset_y = center_offset_y + (height_diff / -2.0f);
}


void Renderer::calculate_required_zoom(const std::vector<Coordinates>& duck_positions) {
    if (duck_positions.empty()) {
        zoom_factor = 1.0f;
        return;
    }

    // Find the leftmost and rightmost duck positions
    float min_x = duck_positions[0].pos_X;
    float max_x = duck_positions[0].pos_X;
    float min_y = duck_positions[0].pos_Y;
    float max_y = duck_positions[0].pos_Y;

    for (const auto& pos : duck_positions) {
        min_x = std::min(min_x, pos.pos_X);
        max_x = std::max(max_x, pos.pos_X);
        min_y = std::min(min_y, pos.pos_Y);
        max_y = std::max(max_y, pos.pos_Y);
    }

    // Add more generous padding (multiply the sprite size by a factor)
    const float PADDING_FACTOR = 4.0f;  // Increased padding
    float width_needed = (max_x - min_x) + DUCK_SPRITE_WIDTH * PADDING_FACTOR;
    float height_needed = (max_y - min_y) + DUCK_SPRITE_HEIGHT * PADDING_FACTOR;

    // Calculate required zoom to fit all ducks
    float zoom_x = window.GetWidth() / width_needed;
    float zoom_y = window.GetHeight() / height_needed;

    // Use the smaller zoom factor and apply a safety margin
    zoom_factor = std::min(zoom_x, zoom_y) * 0.8f;  // 80% of calculated zoom for extra safety margin

    // Adjust zoom limits - make minimum zoom smaller
    const float MIN_ZOOM = 0.1f;  // Decreased minimum zoom
    const float MAX_ZOOM = 1.0f;
    zoom_factor = std::max(MIN_ZOOM, std::min(zoom_factor, MAX_ZOOM));

    // Optional: Add smooth transition to avoid sudden zoom changes
    static float last_zoom = 1.0f;
    const float SMOOTH_FACTOR = 0.1f;
    zoom_factor = last_zoom + (zoom_factor - last_zoom) * SMOOTH_FACTOR;
    last_zoom = zoom_factor;
}

void Renderer::dibujar_mapa(const float zoom_offset_x, const float zoom_offset_y) {
    YAML::Node config = YAML::LoadFile("resources/mapa_1.yaml");
    if (!config["entities"] || !config["entities"].IsSequence()) {
        throw std::runtime_error("Error al leer entities en el archivo YAML");
    }

    for (const auto& entity : config["entities"]) {
        float x = entity["x"].as<float>();
        float y = entity["y"].as<float>();
        float width = entity["width"].as<float>();
        float height = entity["height"].as<float>();
        
        SDL2pp::Rect rect(
            static_cast<int>(x + zoom_offset_x),
            static_cast<int>(renderer.GetOutputHeight() + zoom_offset_y - y - height),
            static_cast<int>(width),
            static_cast<int>(height)
        );

        if (entity["texture"].IsDefined()) {
            std::string textureName = entity["texture"].as<std::string>();
            SDL2pp::Texture* texture = textureManager.getTextura(textureName);
            
            if (texture) {
                renderer.Copy(*texture, SDL2pp::NullOpt, rect);
            } else {
                renderer.SetDrawColor(108, 59, 42);
                renderer.FillRect(rect);
            }
        } else {
            renderer.SetDrawColor(108, 59, 42);
            renderer.FillRect(rect);
        }
    }
}



void Renderer::run(int frame) {
    try {
        SDL2pp::Texture background(renderer, "resources/fondo.png");
        SDL2pp::Surface tempSurface("resources/Duck-removebg-preview.png");
        SDL2pp::Texture sprites(renderer, tempSurface);
        
        Gamestate update;
        while (updates_feed.try_pop(update)) {
            state.update(update);
        }

        renderer.Clear();

        // Get duck positions
        std::list<Character> character_list = state.get_characters_data();
        std::vector<Coordinates> duck_positions;
        float sum_x = 0.0f;
        float sum_y = 0.0f;
        
        for (auto& character : character_list) {
            duck_positions.emplace_back(character.pos_X, character.pos_Y);
            sum_x += character.pos_X;
            sum_y += character.pos_Y;
        }
        
        // Calculate average positions for both X and Y
        float avg_x = duck_positions.empty() ? 0.0f : sum_x / duck_positions.size();
        float avg_y = duck_positions.empty() ? 0.0f : sum_y / duck_positions.size();

        calculate_required_zoom(duck_positions);
        
        // Calculate offsets for both centering and zooming
        float zoom_offset_x, zoom_offset_y;
        calculate_zoom_offsets(zoom_offset_x, zoom_offset_y, avg_x, avg_y);

        // Apply zoom
        renderer.SetScale(this->zoom_factor, this->zoom_factor);
        
        // Draw background
        renderer.Copy(background, SDL2pp::Rect(0, 0, window.GetWidth(), window.GetHeight()));
    
        // Draw map and characters with calculated offsets
        dibujar_mapa(zoom_offset_x, zoom_offset_y);

        for (auto& character : character_list) {
            draw_character(sprites, character, frame, zoom_offset_x, zoom_offset_y);
        }
        
        renderer.Present();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught in the renderer thread: " << e.what() << '\n';
    }
    catch (...) {
        std::cerr << "Unknown exception on the renderloop.\n";
    }
}
#include "renderer.h"

#include <list>
#include <iostream>
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
const int DUCK_SPRITE_HEIGHT = 70;
const int DUCK_MOVEMENT_SPRITES_LINE = 0;


// Renderer::Renderer(std::atomic_bool& con_stat, SDL2pp::Window& w, SDL2pp::Renderer& r, Queue<Gamestate>& q, StateManager& s): connected(con_stat), window(w), renderer(r), updates_feed(q), state(s) {}
Renderer::Renderer(std::atomic_bool& con_stat, Queue<Gamestate>& q, StateManager& s):
        connected(con_stat),
        window("Duck Game",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            640, 480,
            SDL_WINDOW_RESIZABLE),
        renderer(window, -1, SDL_RENDERER_ACCELERATED),
        updates_feed(q),
        state(s),
        textureManager(renderer)
        {}



void Renderer::draw_character(Character& character, int frame, const float zoom_offset_x, const float zoom_offset_y)
{
    SDL2pp::Texture* sprite;
    
    if (character.is_jumping) {

        sprite = textureManager.getDuckSpriteVolando(character.id);
    } else {
        sprite = textureManager.getDuckSprite(character.id);
    }
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

    SDL_RenderCopyEx(renderer.Get(), sprite->Get(), &src_rect, &dst_rect, 0.0, nullptr, flip);
}

void Renderer::draw_gun(Gun& gun, const float zoom_offset_x, const float zoom_offset_y) {
	SDL2pp::Texture* sprite;
	switch (gun.type) {
		case 5:
			sprite = textureManager.getGunSprite("ak47");
			break;
		case 6:
		case 7:
		case 8:
			sprite = textureManager.getGunSprite("pistolas");
			break;
		default:
			sprite = textureManager.getGunSprite("sniper");
	}
	int vcenter = renderer.GetOutputHeight();
	SDL_RendererFlip flip = gun.pointing_to_the_right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_Rect src_rect = search_sprite(gun.type);
    SDL_Rect dst_rect = search_dimension_sprite(vcenter, gun, zoom_offset_x, zoom_offset_y);
    SDL_RenderCopyEx(renderer.Get(), sprite->Get(), &src_rect, &dst_rect, 0.0, nullptr, flip);
}

void Renderer::draw_bullet(Bullet& bullet,  const float zoom_offset_x, const float zoom_offset_y) {	
	std::cout<<"antes del fault"<<std::endl;
	SDL2pp::Texture* sprite = textureManager.getGunSprite("pistolas");
	int vcenter = renderer.GetOutputHeight();
	SDL_RendererFlip flip = bullet.moving_right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_Rect src_rect = { 1, 89, 16, 16 };
    SDL_Rect dst_rect = { static_cast<int>(bullet.pos_X + zoom_offset_x), static_cast<int>(vcenter - 47 - bullet.pos_Y + zoom_offset_y), 16, 16 };
    SDL_RenderCopyEx(renderer.Get(), sprite->Get(), &src_rect, &dst_rect, 0.0, nullptr, flip);
}

SDL_Rect Renderer::search_sprite(const int type) {
	switch (type) {
		case 5:
			return {0, 0, 32, 32};
		case 6:
			return {75, 152, 32, 32};
		case 7:
			return {1, 20, 22, 11};
		default:
			return {1, 47, 32, 32};
	}
}


SDL_Rect Renderer::search_dimension_sprite(int vcenter, Gun& gun, const float zoom_offset_x, const float zoom_offset_y) {
	switch (gun.type) {
		case 5:
			return { static_cast<int>(gun.pos_X + zoom_offset_x), static_cast<int>(vcenter - 47 - gun.pos_Y + zoom_offset_y), 64, 64 };
		case 6:
			return { static_cast<int>(gun.pos_X + zoom_offset_x), static_cast<int>(vcenter - 47 - gun.pos_Y + zoom_offset_y), 64, 64 };
		case 7:
			return { static_cast<int>(gun.pos_X + zoom_offset_x), static_cast<int>(vcenter - 47 - gun.pos_Y + zoom_offset_y), 44, 22 };
		default:
			return { static_cast<int>(gun.pos_X + zoom_offset_x), static_cast<int>(vcenter - 47 - gun.pos_Y + zoom_offset_y), 64, 64 };
	}
}

void Renderer::calculate_zoom_offsets(float& offset_x, float& offset_y, float avg_x, float avg_y) {
    int window_width = window.GetWidth();
    int window_height = window.GetHeight();
    
    float target_center_x = window_width / 2.0f;
    float target_center_y = window_height * 0.7f;  
    
    float center_offset_x = target_center_x - avg_x;
    
    float center_offset_y = (target_center_y - (window_height - avg_y));
    
    float zoomed_width = window_width * zoom_factor;
    float zoomed_height = window_height * zoom_factor;
    float width_diff = zoomed_width - window_width;
    float height_diff = zoomed_height - window_height;
    
    offset_x = center_offset_x + (width_diff / -2.0f);
    offset_y = center_offset_y + (height_diff / -2.0f);
}


void Renderer::calculate_required_zoom(const std::vector<Coordinates>& duck_positions) {
    if (duck_positions.empty()) {
        zoom_factor = 1.0f;
        return;
    }

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

    const float PADDING_FACTOR = 4.0f;  
    float width_needed = (max_x - min_x) + DUCK_SPRITE_WIDTH * PADDING_FACTOR;
    float height_needed = (max_y - min_y) + DUCK_SPRITE_HEIGHT * PADDING_FACTOR;

    float zoom_x = window.GetWidth() / width_needed;
    float zoom_y = window.GetHeight() / height_needed;

    zoom_factor = std::min(zoom_x, zoom_y) * 0.8f;  

    const float MIN_ZOOM = 0.1f;  
    const float MAX_ZOOM = 1.0f;
    zoom_factor = std::max(MIN_ZOOM, std::min(zoom_factor, MAX_ZOOM));

    static float last_zoom = 1.0f;
    const float SMOOTH_FACTOR = 0.1f;
    zoom_factor = last_zoom + (zoom_factor - last_zoom) * SMOOTH_FACTOR;
    last_zoom = zoom_factor;
}

void Renderer::dibujar_mapa(const float zoom_offset_x, const float zoom_offset_y) {
    YAML::Node config = YAML::LoadFile(getCurrentMap());
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
            } 
        } 
    }
}

std::string Renderer::get_fondo(){
    // Carga el archivo YAML
    YAML::Node config = YAML::LoadFile("resources/current_map.yaml");

    // Verifica si las claves existen
    if (!config["current_map"] || !config["mapa_fondos"]) {
        throw std::runtime_error("Missing 'current_map' or 'mapa_fondos' keys in current_map.yaml");
    }

    // Obtiene el mapa actual
    std::string current_map = config["current_map"].as<std::string>();

    // Obtiene el fondo asociado al mapa
    if (!config["mapa_fondos"][current_map]) {
        throw std::runtime_error("Background not found for map: " + current_map);
    }

    std::string fondo = config["mapa_fondos"][current_map].as<std::string>();

    return "resources/" + fondo;
}


void Renderer::render(int frame) {
    try {
        if (not connected.load()) {
            SDL_Quit();
            return;
        }
        std::string fondoPath = get_fondo();
        SDL2pp::Texture background(renderer, fondoPath);
  
        Gamestate update;
        while (updates_feed.try_pop(update)) {
            state.update(update);
        }

        renderer.Clear();
        std::list<Character> character_list = state.get_characters_data();
        std::list<Gun> gun_list = state.get_guns_data();
        std::list<Bullet> bullet_list = state.get_bullets_data();
        
        // CALCULO ZOOM Y POSICIONES
        std::vector<Coordinates> duck_positions;
        float sum_x = 0.0f;
        float sum_y = 0.0f;
        
        for (auto& character : character_list) {
            duck_positions.emplace_back(character.pos_X, character.pos_Y);
            sum_x += character.pos_X;
            sum_y += character.pos_Y;
        }
        
        float avg_x = duck_positions.empty() ? 0.0f : sum_x / duck_positions.size();
        float avg_y = duck_positions.empty() ? 0.0f : sum_y / duck_positions.size();

        calculate_required_zoom(duck_positions);
        
        float zoom_offset_x, zoom_offset_y;
        calculate_zoom_offsets(zoom_offset_x, zoom_offset_y, avg_x, avg_y);

        renderer.SetScale(this->zoom_factor, this->zoom_factor);
        
        renderer.Copy(background, SDL2pp::Rect(0, 0, window.GetWidth(), window.GetHeight()));
    

        // DiIBUJO MAPA
        dibujar_mapa(zoom_offset_x, zoom_offset_y);

        // DIBUJO PATOS
        for (auto& character : character_list) {
            // if (character.is_alive)
                draw_character(character, frame, zoom_offset_x, zoom_offset_y);
        }
        
        // DIBUJO ARMAS
        for (auto& gun : gun_list) {
        	draw_gun(gun, zoom_offset_x, zoom_offset_y);
        }
        
        // DIBUJO BALAS
        for (auto& bullet : bullet_list) {
        	std::cout<<bullet.id<<"- x:"<<bullet.pos_X<<" y: "<<bullet.pos_Y<<std::endl;
        	draw_bullet(bullet, zoom_offset_x, zoom_offset_y);
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

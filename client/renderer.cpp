#include "renderer.h"

#include <list>
#include <iostream>
#include <SDL.h>

#include "client/character.h"
#include "client/state_manager.h"
#include "client/caja.h"
#include "SDL2pp/Renderer.hh"
#include "SDL2pp/SDL.hh"
#include "SDL2pp/Surface.hh"
#include "SDL2pp/Texture.hh"
#include "SDL2pp/Window.hh"
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <vector>
#include <fstream>
#include "SDL2pp/Font.hh"

const int DUCK_SPRITE_WIDTH = 64;
const int DUCK_SPRITE_HEIGHT = 70;
const int DUCK_MOVEMENT_SPRITES_LINE = 0;


// Renderer::Renderer(std::atomic_bool& con_stat, SDL2pp::Window& w, SDL2pp::Renderer& r, Queue<Gamestate>& q, StateManager& s): connected(con_stat), window(w), renderer(r), updates_feed(q), state(s) {}
Renderer::Renderer(std::atomic_bool& con_stat, Queue<Gamestate>& q, StateManager& s):
        connected(con_stat),
        window("Duck Game",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            640, 480,
            SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE),
        renderer(window, -1, SDL_RENDERER_ACCELERATED),
        updates_feed(q),
        state(s),
        textureManager(renderer)
        {}

void Renderer::draw_character(Character& character, int frame, const float zoom_offset_x, const float zoom_offset_y)
{
    SDL2pp::Texture* sprite;
    if (character.is_ducking) {
        sprite = textureManager.getDuckSpriteDucking(character.id);
    } else if (character.is_jumping) {
        sprite = textureManager.getDuckSpriteVolando(character.id);
    } else {
        sprite = textureManager.getDuckSprite(character.id, character.color);
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

    if (true){ // poner aca las condiciones tiene casco o no, tiene armadura o no

        SDL2pp::Texture* armorTexture = textureManager.getItem("armadura");
        if (armorTexture) {
            SDL_Rect armor_src_rect = { 0, 0, DUCK_SPRITE_WIDTH, DUCK_SPRITE_HEIGHT };
            SDL_Rect armor_dst_rect = {
                static_cast<int>(character.pos_X + zoom_offset_x) + 5,
                static_cast<int>(vcenter - DUCK_SPRITE_HEIGHT - character.pos_Y + zoom_offset_y + 15),
                DUCK_SPRITE_WIDTH -10,
                DUCK_SPRITE_HEIGHT -10
            };
            SDL_RenderCopyEx(renderer.Get(), armorTexture->Get(), &armor_src_rect, &armor_dst_rect, 0.0, nullptr, flip);
        }

        SDL2pp::Texture* cascoTexture = textureManager.getItem("casco");
        if (cascoTexture) {
            SDL_Rect armor_src_rect = { 0, 0, DUCK_SPRITE_WIDTH, DUCK_SPRITE_HEIGHT };
            int casco_width = DUCK_SPRITE_WIDTH -20;  // Ajusta el factor de reducción (por ejemplo, la mitad del tamaño)
            int casco_height = DUCK_SPRITE_HEIGHT -20;
            SDL_Rect armor_dst_rect = {
                static_cast<int>(character.pos_X + zoom_offset_x + (DUCK_SPRITE_WIDTH - casco_width) / 2 ), // Centrar el casco en X
                static_cast<int>(vcenter - DUCK_SPRITE_HEIGHT - character.pos_Y + zoom_offset_y - 15 + (DUCK_SPRITE_HEIGHT - casco_height) / 2), // Ajustar en Y
                casco_width,
                casco_height
            };
            SDL_RenderCopyEx(renderer.Get(), cascoTexture->Get(), &armor_src_rect, &armor_dst_rect, 0.0, nullptr, flip);
        }
    }

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
			break;
	}
	int vcenter = renderer.GetOutputHeight();
	SDL_RendererFlip flip = gun.pointing_to_the_right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_Rect src_rect = search_sprite(gun.type);
    SDL_Rect dst_rect = search_dimension_sprite(vcenter, gun, zoom_offset_x, zoom_offset_y);
    SDL_RenderCopyEx(renderer.Get(), sprite->Get(), &src_rect, &dst_rect, 0.0, nullptr, flip);
}

void Renderer::draw_bullet(Bullet& bullet,  const float zoom_offset_x, const float zoom_offset_y) {
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
		case 8:
			return {1, 47, 32, 32};
		default:
			return {0, 0, 33, 9};
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
		case 8:
			return { static_cast<int>(gun.pos_X + zoom_offset_x), static_cast<int>(vcenter - 47 - gun.pos_Y + zoom_offset_y), 64, 64 };
		default:
			return { static_cast<int>(gun.pos_X + zoom_offset_x), static_cast<int>(vcenter - 47 - gun.pos_Y + zoom_offset_y), 66, 18 };
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

void Renderer::dibujar_cajas(const float zoom_offset_x, const float zoom_offset_y) {
    YAML::Node config = YAML::LoadFile(getCurrentMap());

    std::vector<Caja> cajas;


    for (const auto& spawn : config["spawn_places_cajas"]) {
        float x = spawn["x"].as<float>();
        float y = spawn["y"].as<float>();
        cajas.emplace_back(x, y); // Crear una nueva instancia de Caja y agregarla al vector
    }

    for (const auto& caja : cajas) {
        
        float width = 50.0f;
        float height = 50.0f;

        SDL2pp::Rect rect(
            static_cast<int>(caja.x + zoom_offset_x),
            static_cast<int>(renderer.GetOutputHeight() + zoom_offset_y - caja.y - height),
            static_cast<int>(width),
            static_cast<int>(height)
        );

        
        SDL2pp::Texture* texture = textureManager.getItem("caja");

        if (texture) {
            renderer.Copy(*texture, SDL2pp::NullOpt, rect);
        } 
    }
}


void Renderer::draw_bg(){
    YAML::Node config = YAML::LoadFile("resources/current_map.yaml");

    std::string mapa_actual;
    mapa_actual = config["current_map"].as<std::string>();


    SDL2pp::Texture* texture = textureManager.getTextura(mapa_actual);

    if (texture) {
        // Dibujar la textura
        renderer.Copy(*texture, SDL2pp::Rect(0, 0, window.GetWidth(), window.GetHeight()));
    } else {
        std::cerr << "Error: No se encontró la textura para el mapa: " << mapa_actual << std::endl;
    }
    
}


void Renderer::render(int frame) {
    try {
        if (not connected.load()) {
            // SDL_Quit();
            // TTF_Quit();
            return;
        }

        renderer.Clear();

        draw_bg();
  
        Gamestate update;
        while (updates_feed.try_pop(update)) {
            state.update(update);
        }

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

        // DiIBUJO MAPA
        dibujar_mapa(zoom_offset_x, zoom_offset_y);

        dibujar_cajas(zoom_offset_x, zoom_offset_y);

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
        	draw_bullet(bullet, zoom_offset_x, zoom_offset_y);
        }

        renderer.SetScale(1.0f, 1.0f);
        SDL2pp::Font font("resources/Uroob-Regular.ttf", 24);
        SDL_Color color = {255, 255, 255, 255};
        SDL2pp::Texture text(renderer, font.RenderText_Solid("Score: 0", color));
        SDL2pp::Texture text1(renderer, font.RenderText_Solid("Score: 0", color));
        renderer.Copy(text, SDL2pp::NullOpt, SDL2pp::Rect(50, 50, text.GetWidth(), text.GetHeight()));
        renderer.Copy(text1, SDL2pp::NullOpt, SDL2pp::Rect(50, 75, text.GetWidth(), text.GetHeight()));
        renderer.Present();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught in the renderer thread: " << e.what() << '\n';
    }
    catch (...) {
        std::cerr << "Unknown exception on the renderloop.\n";
    }
}

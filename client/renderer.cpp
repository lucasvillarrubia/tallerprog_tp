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
#include <SDL2/SDL_mixer.h>

#include "state.h"

const int DUCK_SPRITE_WIDTH = 64;
const int DUCK_SPRITE_HEIGHT = 70;
const int DUCK_MOVEMENT_SPRITES_LINE = 0;


// Renderer::Renderer(std::atomic_bool& con_stat, SDL2pp::Window& w, SDL2pp::Renderer& r, Queue<Gamestate>& q, StateManager& s): connected(con_stat), window(w), renderer(r), updates_feed(q), state(s) {}
Renderer::Renderer(std::atomic_bool& con_stat, Queue<Gamestate>& q, StateManager& s, const int& player_count):
        connected(con_stat),
        window("Duck Game",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            640, 480,
            SDL_WINDOW_HIDDEN),
            // SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE),
        renderer(window, -1, SDL_RENDERER_ACCELERATED),
        updates_feed(q),
        state(s),
        textureManager(renderer),
        player_count(player_count)
        // original_window_width(original_window_width),
        // original_window_height(original_window_height)
    {
        original_window_height = renderer.GetOutputHeight();
        original_window_width = renderer.GetOutputWidth();
        config_bg = YAML::LoadFile("resources/current_map.yaml");
        config_map = YAML::LoadFile(getCurrentMap());
        // load_background();
        std::string mapa_actual;
        mapa_actual = config_bg["current_map"].as<std::string>();
        bg_texture = textureManager.getTextura(mapa_actual);
    }

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
		case 1:
		case 2:
			sprite = textureManager.getGunSprite("arrojadizos");
			break;
		case 3:
		case 4:
			sprite = textureManager.getGunSprite("lasers");
			break;
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
	float angle = gun.pointing_up ? 90.0f : 0.0f;
	angle = angle * (gun.pointing_to_the_right ? -1 : 1);
	SDL_RendererFlip flip = gun.pointing_to_the_right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_Rect src_rect = search_sprite(gun);
    SDL_Rect dst_rect = search_dimension_sprite(vcenter, gun, zoom_offset_x, zoom_offset_y);
    SDL_RenderCopyEx(renderer.Get(), sprite->Get(), &src_rect, &dst_rect, angle, nullptr, flip);
}

void Renderer::draw_bullet(Bullet& bullet, const float zoom_offset_x, const float zoom_offset_y) {
	SDL2pp::Texture* sprite = textureManager.getGunSprite("pistolas");
	int vcenter = renderer.GetOutputHeight();
	float angle = bullet.moving_up ? 90.0f : 0.0f;
	angle = angle * (bullet.moving_right ? -1 : 1);
	SDL_RendererFlip flip = bullet.moving_right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_Rect src_rect = { 1, 89, 16, 16 };
    SDL_Rect dst_rect = { static_cast<int>(bullet.pos_X + zoom_offset_x), static_cast<int>(vcenter - 47 - bullet.pos_Y + zoom_offset_y), 16, 16 };
    SDL_RenderCopyEx(renderer.Get(), sprite->Get(), &src_rect, &dst_rect, angle, nullptr, flip);
}

void Renderer::draw_explosion(Explosion& explosion, const float zoom_offset_x, const float zoom_offset_y) {
	SDL2pp::Texture* sprite = textureManager.getGunSprite("explosion");
	int vcenter = renderer.GetOutputHeight();
    SDL_Rect src_rect = {explosion.current_state*64,0,64,64};
    SDL_Rect dst_rect = { static_cast<int>(explosion.pos_X + zoom_offset_x), static_cast<int>(vcenter - 47 - explosion.pos_Y + zoom_offset_y), 64, 64 };
    SDL_RenderCopyEx(renderer.Get(), sprite->Get(), &src_rect, &dst_rect, 0.0, nullptr, SDL_FLIP_NONE);
}

SDL_Rect Renderer::search_sprite(Gun& gun) {
	switch (gun.type) {
		case 1:
			if (gun.shooting) {
				return {1,21,16,16};
			} else {
				return {17,21,16,16};
			}
		case 2:
			if (gun.shooting) {
				return {1,67,16,16};
			} else {
				return {1,99,16,16};
			}
		case 3:
			return {336, 94, 32, 32};
		case 4:
			return {1, 97, 32, 32};
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
		case 1:
			return { static_cast<int>(gun.pos_X + 16 + zoom_offset_x), static_cast<int>(vcenter - 48 - gun.pos_Y + zoom_offset_y), 32, 32 };
		case 2:
			return { static_cast<int>(gun.pos_X + 16 + zoom_offset_x), static_cast<int>(vcenter - 48 - gun.pos_Y + zoom_offset_y), 32, 32 };
		case 3:
			return { static_cast<int>(gun.pos_X + zoom_offset_x), static_cast<int>(vcenter - 64 - gun.pos_Y + zoom_offset_y), 64, 64 };
		case 4:
			return { static_cast<int>(gun.pos_X + zoom_offset_x), static_cast<int>(vcenter - 64 - gun.pos_Y + zoom_offset_y), 64, 64 };
		case 5:
			return { static_cast<int>(gun.pos_X + zoom_offset_x), static_cast<int>(vcenter - 64 - gun.pos_Y + zoom_offset_y), 64, 64 };
		case 6:
			return { static_cast<int>(gun.pos_X + zoom_offset_x), static_cast<int>(vcenter - 64 - gun.pos_Y + zoom_offset_y), 64, 64 };
		case 7:
			return { static_cast<int>(gun.pos_X + zoom_offset_x), static_cast<int>(vcenter - 47 - gun.pos_Y + zoom_offset_y), 44, 22 };
		case 8:
			return { static_cast<int>(gun.pos_X + zoom_offset_x), static_cast<int>(vcenter - 64 - gun.pos_Y + zoom_offset_y), 64, 64 };
		default:
			return { static_cast<int>(gun.pos_X + zoom_offset_x), static_cast<int>(vcenter - 47 - gun.pos_Y + zoom_offset_y), 66, 18 };
	}
}

// void Renderer::calculate_zoom_offsets(float& offset_x, float& offset_y, float avg_x, float avg_y) {
//     int window_width = original_window_width;
//     int window_height = original_window_height;
    
//     float target_center_x = window_width / 2.0f;
//     float target_center_y = window_height * 0.7f;  
    
//     float center_offset_x = target_center_x - avg_x;
    
//     float center_offset_y = (target_center_y - (window_height - avg_y));
    
//     float zoomed_width = window_width * zoom_factor;
//     float zoomed_height = window_height * zoom_factor;
//     float width_diff = zoomed_width - window_width;
//     float height_diff = zoomed_height - window_height;
    
//     offset_x = center_offset_x + (width_diff / -2.0f);
//     offset_y = center_offset_y + (height_diff / -2.0f);
// }

void Renderer::calculate_zoom_offsets(float& offset_x, float& offset_y, 
                                    const float avg_x, const float avg_y) {
    float screen_center_x = original_window_width * 0.5f;
    float screen_center_y = original_window_height * 0.5f;

    offset_x = screen_center_x - (avg_x * zoom_factor);
    offset_y = screen_center_y - ((original_window_height - avg_y) * zoom_factor);
}

void Renderer::calculate_required_zoom() {
    // if (duck_positions.empty()) {
    //     zoom_factor = 1.0f;
    //     return;
    // }

    // // Single pass through positions
    // auto& state = zoom_state;
    // state.min_x = state.max_x = duck_positions[0].pos_X;
    // state.min_y = state.max_y = duck_positions[0].pos_Y;

    // for (const auto& pos : duck_positions) {
    //     // Branchless min/max
    //     state.min_x += (pos.pos_X - state.min_x) * (pos.pos_X < state.min_x);
    //     state.max_x += (pos.pos_X - state.max_x) * (pos.pos_X > state.max_x);
    //     state.min_y += (pos.pos_Y - state.min_y) * (pos.pos_Y < state.min_y);
    //     state.max_y += (pos.pos_Y - state.max_y) * (pos.pos_Y > state.max_y);
    // }

    // // Combine calculations
    // const float width_scale = original_window_width / 
    //     ((state.max_x - state.min_x) + DUCK_SPRITE_WIDTH * PADDING_FACTOR);
    // const float height_scale = original_window_height / 
    //     ((state.max_y - state.min_y) + DUCK_SPRITE_HEIGHT * PADDING_FACTOR);

    // // Calculate new zoom
    // const float new_zoom = std::min(width_scale, height_scale) * 0.8f;
    // const float clamped_zoom = std::clamp(new_zoom, MIN_ZOOM, MAX_ZOOM);
    
    // // Apply smoothing
    // zoom_factor = state.last_zoom + (clamped_zoom - state.last_zoom) * SMOOTH_FACTOR;
    // state.last_zoom = zoom_factor;

    if (duck_positions.empty()) {
        zoom_factor = 1.0f;
        return;
    }

    // Find bounds
    auto& state = zoom_state;
    state.min_x = state.max_x = duck_positions[0].pos_X;
    state.min_y = state.max_y = duck_positions[0].pos_Y;

    state.min_y = 600.0f;
    state.max_y = -400.0f;
    state.min_x = 1200.0f;
    state.max_x = -300.0f; 

    for (const auto& pos : duck_positions) {
        state.min_x = std::min(state.min_x, pos.pos_X);
        state.max_x = std::max(state.max_x, pos.pos_X);
        state.min_y = std::min(state.min_y, pos.pos_Y);
        state.max_y = std::max(state.max_y, pos.pos_Y);
    }

    // Add padding
    float width = (state.max_x - state.min_x) + PADDING;
    float height = (state.max_y - state.min_y) + PADDING;

    // viewport_x = state.min_x;
    // viewport_y = state.min_y;
    // viewport_width = width - PADDING;
    // viewport_height = height - PADDING;

    // Calculate required zoom
    float zoom_x = original_window_width / width;
    float zoom_y = original_window_height / height;
    state.target_zoom = std::min(zoom_x, zoom_y);
    std::cout << "target_zoom: " << state.target_zoom << std::endl;
    
    // Clamp and smooth
    state.target_zoom = std::clamp(state.target_zoom, MIN_ZOOM, MAX_ZOOM);
    zoom_factor = state.current_zoom + 
                  (state.target_zoom - state.current_zoom) * ZOOM_SMOOTH;
    state.current_zoom = zoom_factor;
}

// void Renderer::calculate_required_zoom(const std::vector<Coordinates>& duck_positions) {
// void Renderer::calculate_required_zoom() {
//     if (duck_positions.empty()) {
//         zoom_factor = 1.0f;
//         return;
//     }

//     float min_x = duck_positions[0].pos_X;
//     float max_x = duck_positions[0].pos_X;
//     float min_y = duck_positions[0].pos_Y;
//     float max_y = duck_positions[0].pos_Y;

//     for (const auto& pos : duck_positions) {
//         min_x = std::min(min_x, pos.pos_X);
//         max_x = std::max(max_x, pos.pos_X);
//         min_y = std::min(min_y, pos.pos_Y);
//         max_y = std::max(max_y, pos.pos_Y);
//     }

//     const float PADDING_FACTOR = 4.0f;  
//     float width_needed = (max_x - min_x) + DUCK_SPRITE_WIDTH * PADDING_FACTOR;
//     float height_needed = (max_y - min_y) + DUCK_SPRITE_HEIGHT * PADDING_FACTOR;

//     float zoom_x = original_window_width / width_needed;
//     float zoom_y = original_window_height / height_needed;

//     zoom_factor = std::min(zoom_x, zoom_y) * 0.8f;  

//     const float MIN_ZOOM = 0.1f;  
//     const float MAX_ZOOM = 1.0f;
//     zoom_factor = std::max(MIN_ZOOM, std::min(zoom_factor, MAX_ZOOM));

//     static float last_zoom = 1.0f;
//     const float SMOOTH_FACTOR = 0.1f;
//     zoom_factor = last_zoom + (zoom_factor - last_zoom) * SMOOTH_FACTOR;
//     last_zoom = zoom_factor;
// }



void Renderer::dibujar_mapa(const float zoom_offset_x, const float zoom_offset_y) {
    // YAML::Node config_map = YAML::LoadFile(getCurrentMap());
    if (!config_map["entities"] || !config_map["entities"].IsSequence()) {
        throw std::runtime_error("Error al leer entities en el archivo YAML");
    }

    for (const auto& entity : config_map["entities"]) {
        float x = entity["x"].as<float>();
        float y = entity["y"].as<float>();
        float width = entity["width"].as<float>();
        float height = entity["height"].as<float>();
        
        SDL2pp::Rect rect(
            static_cast<int>(x + zoom_offset_x),
            // static_cast<int>(renderer.GetOutputHeight() + zoom_offset_y - y - height),
            static_cast<int>(original_window_height + zoom_offset_y - y - height),
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
    // YAML::Node config_bg = YAML::LoadFile("resources/current_map.yaml");

    // std::string mapa_actual;
    // mapa_actual = config_bg["current_map"].as<std::string>();


    // SDL2pp::Texture* texture = textureManager.getTextura(mapa_actual);

    if (bg_texture) {
        // Dibujar la textura
        renderer.Copy(*bg_texture, SDL2pp::Rect(0, 0, original_window_width, original_window_height));
    } else {
        std::cerr << "Error: No se encontró la textura para el mapa: " << std::endl;
    }
    
}

void Renderer::playJumpSound(const std::string& soundName, int volume) {
    textureManager.playPreloadedSound(soundName, volume);
}

void Renderer::load_background(){
    // Create a render texture
    map_texture = std::make_unique<SDL2pp::Texture>(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, original_window_width + 1000.0f, original_window_height + 1333.3f
    );

    // Set the render target to the map texture
    renderer.SetTarget(*map_texture);

    // Clear the texture
    renderer.SetDrawColor(0, 0, 0, 0);
    renderer.Clear();

    // Draw the background
    std::string mapa_actual = config_bg["current_map"].as<std::string>();
    SDL2pp::Texture* bg_texture = textureManager.getTextura(mapa_actual);
    if (bg_texture) {
        renderer.Copy(*bg_texture, SDL2pp::Rect(0, 0, original_window_width + 600.0f, original_window_height + 450.0f));
    } else {
        std::cerr << "Error: No se encontró la textura para el mapa: " << mapa_actual << std::endl;
    }

    // Draw the map entities
    if (!config_map["entities"] || !config_map["entities"].IsSequence()) {
        throw std::runtime_error("Error al leer entities en el archivo YAML");
    }
    for (const auto& entity : config_map["entities"]) {
        float x = entity["x"].as<float>();
        float y = entity["y"].as<float>();
        float width = entity["width"].as<float>();
        float height = entity["height"].as<float>();

        SDL2pp::Rect rect(
            static_cast<int>(x + 310.0f),
            static_cast<int>(original_window_height + 540.0f - y - height),
            static_cast<int>(width),
            static_cast<int>(height)
        );

        if (entity["texture"].IsDefined()) {
            std::string textureName = entity["texture"].as<std::string>();
            SDL2pp::Texture* entity_texture = textureManager.getTextura(textureName);

            if (entity_texture) {
                renderer.Copy(*entity_texture, SDL2pp::NullOpt, rect);
            }
        }
    }

    // Reset the render target to the default
    renderer.SetTarget();
}

void Renderer::set_draw_settings(const float zoom_offset_x, const float zoom_offset_y) {
// void Renderer::set_draw_settings() {
    // Apply transformations
    renderer.SetScale(1.0f, 1.0f);
    renderer.SetViewport(SDL2pp::Rect(zoom_offset_x - 450.0f, zoom_offset_y - 600.0f, original_window_width / zoom_factor, original_window_height /zoom_factor));
    // renderer.SetViewport(SDL2pp::Rect(-310.0f, 540.0f, original_window_width, original_window_height));

    // Draw the combined texture
    renderer.Copy(*map_texture, SDL2pp::NullOpt, SDL2pp::NullOpt);
    renderer.SetScale(zoom_factor, zoom_factor);
    // Reset transformations
    // renderer.SetScale(1.0f, 1.0f);
    renderer.SetViewport(SDL2pp::NullOpt);
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
  
        // Gamestate update;
        while (updates_feed.try_pop(current_state)) {
            state.update(current_state);
        }


        character_list.clear();
        character_list = state.get_characters_data();
        // std::list<Character> character_list = state.get_characters_data();

        for (const auto& character : character_list) {
            if (character.jump_velocity > 13.0f) {
                
                playJumpSound("salto", 128);
            }
        }


        // State current_state = state.get_state();
        // character_list.clear();
        gun_list.clear();
        bullet_list.clear();
        // character_list = state.get_characters_data();
        gun_list = state.get_guns_data();
        bullet_list = state.get_bullets_data();

        std::list<Explosion> explosion_list = state.get_explosions_data();


        
        // std::list<Character> character_list = current_state.dukis;
        // std::list<Gun> gun_list = current_state.guns;
        // std::list<Bullet> bullet_list = current_state.bullets;

        duck_positions.clear();
        // CALCULO ZOOM Y POSICIONES
        // std::vector<Coordinates> duck_positions;
        float sum_x = 0.0f;
        float sum_y = 0.0f;
        
        for (auto& character : character_list) {
            duck_positions.emplace_back(character.pos_X, character.pos_Y);
            sum_x += character.pos_X;
            sum_y += character.pos_Y;
          //  std::cout << "Character jump_velocity: " << character.jump_velocity << std::endl;
        }

        
        
        float avg_x = duck_positions.empty() ? 0.0f : sum_x / duck_positions.size();
        float avg_y = duck_positions.empty() ? 0.0f : sum_y / duck_positions.size();

        // calculate_required_zoom(duck_positions);
        calculate_required_zoom();
        // duck_positions.clear();
        
        float zoom_offset_x, zoom_offset_y;
        calculate_zoom_offsets(zoom_offset_x, zoom_offset_y, avg_x, avg_y);

        // set_draw_settings(zoom_offset_x, zoom_offset_y);
        // set_draw_settings();
        renderer.SetScale(this->zoom_factor, this->zoom_factor);

        // std::cout << "Zoom: " << zoom_factor 
        //       << " Offset: (" << zoom_offset_x << "," << zoom_offset_y << ")\n";

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

        // DIBUJO EXPLOSIONES
        for (auto& explosion : explosion_list) {
        	draw_explosion(explosion, zoom_offset_x, zoom_offset_y);
        }
        state.set_explosion_phase(frame);

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

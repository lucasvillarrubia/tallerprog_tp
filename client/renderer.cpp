#include "renderer.h"

#include <list>
#include <SDL.h>
#include <iostream>

#include "client/character.h"
#include "client/state_manager.h"
#include "SDL2pp/Renderer.hh"
#include "SDL2pp/SDL.hh"
#include "SDL2pp/Surface.hh"
#include "SDL2pp/Texture.hh"
#include "SDL2pp/Window.hh"


const int DUCK_SPRITE_WIDTH = 64;
const int DUCK_SPRITE_HEIGHT = 64;
const int DUCK_MOVEMENT_SPRITES_LINE = 0;


Renderer::Renderer(std::atomic_bool& con_stat, SDL2pp::Window& w, SDL2pp::Renderer& r, Queue<Gamestate>& q, StateManager& s): connected(con_stat), window(w), renderer(r), updates_feed(q), state(s),
	background(renderer, "resources/fondo.png"),
	duck_surface("resources/Duck.png"),
    duck_sprites(renderer, duck_surface),
    ak_47_surface("resources/ak47.png"),
    ak_47_sprites(renderer, ak_47_surface),
    pistol_surface("resources/PC Computer - Duck Game - Pistol.png"),
    pistol_sprites(renderer, pistol_surface) {
	
}

void Renderer::draw_character(SDL2pp::Texture& sprites, Character& character, int frame)
{
    int vcenter = renderer.GetOutputHeight();
    int src_x = DUCK_SPRITE_WIDTH * character.get_movement_phase(frame);
    int src_y = DUCK_MOVEMENT_SPRITES_LINE;
    SDL_RendererFlip flip = character.moving_right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_Rect src_rect = { src_x, src_y, DUCK_SPRITE_WIDTH, DUCK_SPRITE_HEIGHT };
    SDL_Rect dst_rect = { static_cast<int>(character.pos_X), static_cast<int>(vcenter - 63 - character.pos_Y), 64, 64 };
    SDL_RenderCopyEx(renderer.Get(), sprites.Get(), &src_rect, &dst_rect, 0.0, nullptr, flip);
}


void Renderer::draw_gun(Gun& gun) {
	//int vcenter = renderer.GetOutputHeight();
    //SDL_RendererFlip flip = gun.moving_right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    switch (gun.type) {
    	case 5:
    		draw_ak_47(gun);
    		break;
    	default:
    		draw_pistol(gun);
    		break;
    }
    //SDL2pp::Texture sprite = search_texture(gun.type);
    //SDL_Rect src_rect = search_sprite(gun.type);
    //SDL_Rect dst_rect = search_dimension_sprite(vcenter, gun);
    //SDL_RenderCopyEx(renderer.Get(), sprite.Get(), &src_rect, &dst_rect, 0.0, nullptr, flip);
}

void Renderer::draw_ak_47(Gun& gun) {
	int vcenter = renderer.GetOutputHeight();
	SDL_RendererFlip flip = gun.moving_right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_Rect src_rect = search_sprite(gun.type);
    SDL_Rect dst_rect = search_dimension_sprite(vcenter, gun);
    SDL_RenderCopyEx(renderer.Get(), ak_47_sprites.Get(), &src_rect, &dst_rect, 0.0, nullptr, flip);
	
}

void Renderer::draw_pistol(Gun& gun) {
	int vcenter = renderer.GetOutputHeight();
	SDL_RendererFlip flip = gun.moving_right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_Rect src_rect = search_sprite(gun.type);
    SDL_Rect dst_rect = search_dimension_sprite(vcenter, gun);
    SDL_RenderCopyEx(renderer.Get(), pistol_sprites.Get(), &src_rect, &dst_rect, 0.0, nullptr, flip);
	
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


SDL_Rect Renderer::search_dimension_sprite(int vcenter, Gun& gun) {
	switch (gun.type) {
		case 5:
			return { static_cast<int>(gun.pos_X), static_cast<int>(vcenter - 47 - gun.pos_Y), 64, 64 };
		case 6:
			return { static_cast<int>(gun.pos_X), static_cast<int>(vcenter - 47 - gun.pos_Y), 64, 64 };
		case 7:
			return { static_cast<int>(gun.pos_X), static_cast<int>(vcenter - 47 - gun.pos_Y), 44, 22 };
		default:
			return { static_cast<int>(gun.pos_X), static_cast<int>(vcenter - 47 - gun.pos_Y), 64, 64 };
	}
}

void Renderer::draw_bullet(SDL2pp::Texture& sprites, Bullet& bullet) {
	int vcenter = renderer.GetOutputHeight();
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_Rect src_rect = { 1, 89, 16, 16 };
    SDL_Rect dst_rect = { static_cast<int>(bullet.pos_X), static_cast<int>(vcenter - 47 - bullet.pos_Y), 16, 16 };
    SDL_RenderCopyEx(renderer.Get(), sprites.Get(), &src_rect, &dst_rect, 0.0, nullptr, flip);
	std::cout<<std::to_string(bullet.id)<<": "<<std::to_string(bullet.pos_X)<<std::endl;
}

void Renderer::run(int frame) {
    try
    {
        Gamestate update;
        while (updates_feed.try_pop(update)) {
            state.update(update);
        }
        renderer.Clear();
        renderer.Copy(background, SDL2pp::Rect(0, 0, window.GetWidth(), window.GetHeight()));
        // DIBUJANDO ENTIDADES DE UN MAPA
        SDL2pp::Rect plataforma(120.0f, renderer.GetOutputHeight() - 50.0f, 400.0f, 50.0f);
        SDL2pp::Rect plataforma_izq(0.0f, renderer.GetOutputHeight() - 150.0f - 50.0f, 100.0f, 50.0f);
        SDL2pp::Rect plataforma_der(540.0f, renderer.GetOutputHeight() - 150.0f - 50.0f, 100.0f, 50.0f);
        renderer.SetDrawColor(108, 59, 42);
        renderer.FillRect(plataforma);
        renderer.FillRect(plataforma_izq);
        renderer.FillRect(plataforma_der);
        // DIBUJANDO PERSONAJES
        std::list<Character> character_list = state.get_characters_data();
        std::list<Gun> gun_list = state.get_guns_data();
        std::list<Bullet> bullet_list = state.get_bullets_data();
        for (auto& character : character_list) {
            draw_character(duck_sprites, character, frame);
        }
        for (auto& gun : gun_list) {
        	draw_gun(gun);
        }
        for (auto& bullet : bullet_list) {
        	//std::cout<<std::to_string(bullet.id)<<std::endl;
        	if (!bullet.destroyed){
        		draw_bullet(pistol_sprites, bullet);
        	}
        }
        //std::cout<<std::to_string(bullet_list.size())<<std::endl;
        renderer.Present();
    }
    catch (ClosedQueue const& e)
    {
        std::cerr << "Se cerró la queue del render?! " << e.what() << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught in the renderer thread: " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Unknown exception on the renderloop.\n";
    }
}

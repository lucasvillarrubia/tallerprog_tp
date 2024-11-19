#ifndef RENDERER_H
#define RENDERER_H


#include "client/character.h"
#include "common/gamedata.h"
#include "common/hands_on_threads/queue.h"
#include "common/hands_on_threads/thread.h"
#include "SDL2pp/Renderer.hh"
#include "SDL2pp/Window.hh"
#include "SDL2pp/Surface.hh"
#include "SDL2pp/Texture.hh"
#include "state_manager.h"


class Renderer
{
private:
    std::atomic_bool& connected;
    SDL2pp::Window& window;
    SDL2pp::Renderer& renderer;
    Queue<Gamestate>& updates_feed;
    StateManager& state;
    SDL2pp::Texture background;
    SDL2pp::Surface duck_surface;
    SDL2pp::Texture duck_sprites;
	//SDL2pp::Texture grenade_sprites;
	//SDL2pp::Texture banana_sprites;
	//SDL2pp::Texture pew_pew_lase_sprites;
	//SDL2pp::Texture laser_rifle_sprites;
	//SDL2pp::Texture ak_47_sprites;
	//SDL2pp::Texture duel_pistol_sprites;
	//SDL2pp::Texture cowboy_pistol_sprites;
	SDL2pp::Surface pistol_surface;
	SDL2pp::Texture pistol_sprites;
	//SDL2pp::Texture shotgun_sprites;
	//SDL2pp::Texture sniper_sprites;
	//SDL2pp::Texture armor_sprites;
	//SDL2pp::Texture helmet_sprites;
    // Character duck;
public:
    Renderer(std::atomic_bool&, SDL2pp::Window&, SDL2pp::Renderer&, Queue<Gamestate>&, StateManager&);
    void draw_character(SDL2pp::Texture&, Character&, int);
    void draw_gun(SDL2pp::Texture& sprites, Gun& gun);
    SDL_Rect search_sprite(const int type);
    SDL_Rect search_dimension_sprite(int vcenter, Gun& gun);
    void draw_bullet(SDL2pp::Texture& sprites, Bullet& bullet);
    // void run();
    void run(int);
    ~Renderer() = default;
};


#endif

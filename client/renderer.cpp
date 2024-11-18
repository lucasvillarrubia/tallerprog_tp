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
#include "mapa.h"

const int DUCK_SPRITE_WIDTH = 64;
const int DUCK_SPRITE_HEIGHT = 70;
const int DUCK_MOVEMENT_SPRITES_LINE = 0;


Renderer::Renderer(std::atomic_bool& con_stat, SDL2pp::Window& w, SDL2pp::Renderer& r, Queue<Gamestate>& q, StateManager& s): connected(con_stat), window(w), renderer(r), updates_feed(q), state(s) {}

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

void Renderer::run(int frame, Mapa mapa) {
    try
    {

    //    SDL2pp::Texture background(renderer, "resources/fondo.png");
        // SDL2pp::Surface tempSurface("resources/Duck-removebg-preview.png");


      //  SDL2pp::Surface tempSurface("resources/Duck.png");
     //   SDL2pp::Texture sprites(renderer, tempSurface);

        SDL2pp::Surface tempSurfaceGround("resources/Duck.png");
        SDL2pp::Texture ground_sprites(renderer, tempSurfaceGround);

        SDL2pp::Surface tempSurfaceAir("resources/aleteo.png");
        SDL2pp::Texture air_sprites(renderer, tempSurfaceAir);

        Gamestate update;
        while (updates_feed.try_pop(update)) {
            state.update(update);
        }
        renderer.Clear();
        mapa.dibujar_fondo(renderer, "resources/fondo.png");
        // DIBUJANDO ENTIDADES DE UN MAPA

        mapa.renderizar(this->renderer.Get());

        // DIBUJANDO PERSONAJES
        std::list<Character> character_list = state.get_characters_data();
        for (auto& character : character_list) {
            SDL2pp::Texture& current_sprite = character.is_jumping ? air_sprites : ground_sprites;
            draw_character(current_sprite, character, frame);
        }
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

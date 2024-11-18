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


const int DUCK_SPRITE_WIDTH = 64;
const int DUCK_SPRITE_HEIGHT = 64;
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
        state(s) {}

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

// renderizado de mapa con cámara:
// - mapa en textura completa con foco en una parte del mapa
// rectángulo que engloba a los patos
// solo se renderiza rectángulo con patos

void Renderer::run(int frame) {
    try
    {
        // mover texturas a una clase que esté en el cliente o en el renderer
        SDL2pp::Texture background(renderer, "resources/fondo.png");
        // SDL2pp::Surface tempSurface("resources/Duck-removebg-preview.png");
        SDL2pp::Surface tempSurface("resources/Duck.png");
        SDL2pp::Texture sprites(renderer, tempSurface);
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
        for (auto& character : character_list) {
            draw_character(sprites, character, frame);
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

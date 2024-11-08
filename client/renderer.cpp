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


Renderer::Renderer(std::atomic_bool& con_stat, SDL2pp::Window& w, SDL2pp::Renderer& r, Queue<Gamestate>& q, StateManager& s): connected(con_stat), window(w), renderer(r), updates_feed(q), state(s) {}

void Renderer::draw_character(SDL2pp::Texture& sprites, Character character)
{
    unsigned int frame_ticks = SDL_GetTicks();
    int vcenter = renderer.GetOutputHeight() / 2;
    int src_x = DUCK_SPRITE_WIDTH * character.get_movement_phase(frame_ticks);
    int src_y = DUCK_MOVEMENT_SPRITES_LINE;
    // Coordinates duck_position = character.get_coordinates();
    // SDL_RendererFlip flip = character.is_moving_to_the_right() ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_RendererFlip flip = character.moving_right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_Rect src_rect = { src_x, src_y, DUCK_SPRITE_WIDTH, DUCK_SPRITE_HEIGHT };
    // SDL_Rect dst_rect = { static_cast<int>(duck_position.pos_X), static_cast<int>(vcenter - 32 - duck_position.pos_Y), 64, 64 };
    SDL_Rect dst_rect = { static_cast<int>(character.pos_X), static_cast<int>(vcenter - 32 - character.pos_Y), 64, 64 };
    SDL_RenderCopyEx(renderer.Get(), sprites.Get(), &src_rect, &dst_rect, 0.0, nullptr, flip);
}

void Renderer::run() {
    try
    {
        SDL2pp::Texture background(renderer, "resources/fondo.png");
        SDL2pp::Surface tempSurface("resources/Duck-removebg-preview.png");
        SDL2pp::Texture sprites(renderer, tempSurface);
        // unsigned int prev_ticks = SDL_GetTicks();
        // Después habría una lista de patos
        // std::cout << "alto: " << renderer.GetOutputHeight() << " y ancho: " << renderer.GetOutputWidth() << "\n";
        // Character duck;
        // if (connected.load())
        // while (true)
        // {
            // unsigned int frame_ticks = SDL_GetTicks();
            // unsigned int frame_delta = frame_ticks - prev_ticks;
            // prev_ticks = frame_ticks;
            Gamestate update;
            // // while
            while (updates_feed.try_pop(update))
            {
                state.update(update);
            //     StateManager::update_duck(duck, update);
            //     // std::cout << "coordenadas pato: x: " << duck.get_coordinates().pos_X << "; y: " << duck.get_coordinates().pos_Y << "\n";
            //
            //     // std::cout << "llegó un estado al renderer!" << "\n";
            }
            // else
            // {
            //     duck.update_position(frame_delta);
            // }
            renderer.Clear();
            renderer.Copy(background, SDL2pp::Rect(0, 0, window.GetWidth(), window.GetHeight()));
            std::list<Character> character_list = state.get_characters_data();
            unsigned int frame_ticks = SDL_GetTicks();
            int vcenter = renderer.GetOutputHeight() / 2;
            for (auto& character : character_list)
            {
                // draw_character(sprites, character);
                int src_x = DUCK_SPRITE_WIDTH * character.get_movement_phase(frame_ticks);
                int src_y = DUCK_MOVEMENT_SPRITES_LINE;
                // Coordinates duck_position = character.get_coordinates();
                // SDL_RendererFlip flip = character.is_moving_to_the_right() ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
                SDL_RendererFlip flip = character.moving_right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
                SDL_Rect src_rect = { src_x, src_y, DUCK_SPRITE_WIDTH, DUCK_SPRITE_HEIGHT };
                // SDL_Rect dst_rect = { static_cast<int>(duck_position.pos_X), static_cast<int>(vcenter - 32 - duck_position.pos_Y), 64, 64 };
                SDL_Rect dst_rect = { static_cast<int>(character.pos_X), static_cast<int>(vcenter - 32 - character.pos_Y), 64, 64 };
                SDL_RenderCopyEx(renderer.Get(), sprites.Get(), &src_rect, &dst_rect, 0.0, nullptr, flip);
            }
            renderer.Present();
            // constante de Rate Loop
            // arreglar frame drop
            SDL_Delay(1);
        // }
    }
    // catch (ClosedQueue const& e)
    // {
    //     std::cerr << "Se cerró la queue del render?! " << e.what() << '\n';
    // }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught in the renderer thread: " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Unknown exception on the renderloop.\n";
    }
}

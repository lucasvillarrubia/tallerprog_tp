#include "client.h"
#include "coordinates.h"
#include "character.h"
#include "state_manager.h"

#include <iostream>
#include <exception>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <SDL_image.h>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/Texture.hh>
#include <SDL2pp/Surface.hh>


using namespace SDL2pp;

const int DUCK_SPRITE_WIDTH = 64;
const int DUCK_SPRITE_HEIGHT = 64;
const int DUCK_MOVEMENT_SPRITES_LINE = 0;


int main() try {
    SDL2pp::SDL sdl(SDL_INIT_VIDEO);
    SDL2pp::Window window(
        "Duck Game",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        640, 480,
        SDL_WINDOW_RESIZABLE
    );
    SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL2pp::Texture background(renderer, "resources/fondo.png");
    SDL2pp::Surface tempSurface("resources/Duck-removebg-preview.png");
    SDL2pp::Texture sprites(renderer, tempSurface);
    unsigned int prev_ticks = SDL_GetTicks();
    // Después habría una lista de patos
    Character duck;
    while (true) {
        unsigned int frame_ticks = SDL_GetTicks();
        unsigned int frame_delta = frame_ticks - prev_ticks;
        prev_ticks = frame_ticks;
        SDL_Event event;
        // Esto se va para el EventListener
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                return 0;
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    return 0;
            }
            StateManager::update_duck_state(duck, event);
        }
        // Esto se va para el Renderer
        duck.update_position(frame_delta);
        int vcenter = renderer.GetOutputHeight() / 2;
        renderer.Clear();
        renderer.Copy(background, SDL2pp::Rect(0, 0, window.GetWidth(), window.GetHeight()));
        int src_x = DUCK_SPRITE_WIDTH * duck.get_movement_phase(frame_ticks);
        int src_y = DUCK_MOVEMENT_SPRITES_LINE;
        Coordinates duck_position = duck.get_coordinates();
        SDL_RendererFlip flip = duck.is_moving_to_the_right() ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
        SDL_Rect src_rect = { src_x, src_y, DUCK_SPRITE_WIDTH, DUCK_SPRITE_HEIGHT };
        SDL_Rect dst_rect = { static_cast<int>(duck_position.pos_X), static_cast<int>(vcenter - 32 - duck_position.pos_Y), 64, 64 };
        SDL_RenderCopyEx(renderer.Get(), sprites.Get(), &src_rect, &dst_rect, 0.0, nullptr, flip);
        renderer.Present();
        SDL_Delay(1);
    }
} catch (std::exception& e)
{
    std::cerr << "Error in client main. " << e.what() << "\n";
    return -1;
}
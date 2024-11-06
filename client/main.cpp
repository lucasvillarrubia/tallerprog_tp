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
#include "mapa.h"
#include "item_box.h"

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

    Mapa mapa(renderer, "resources/mapas/fondo_pantano.png");

    SDL2pp::Surface tempSurface("resources/Duck-removebg-preview.png");
    SDL2pp::Texture sprites(renderer, tempSurface);

    SDL2pp::Surface duckingSurface("resources/pato_ducking.png");
    SDL2pp::Texture duckingSprites(renderer, duckingSurface);

    SDL2pp::Surface pistolSurface("resources/PC Computer - Duck Game - Pistol.png");
    SDL2pp::Texture pistolSprites(renderer, pistolSurface);

    std::vector<ItemBox> boxes;
    boxes.emplace_back(renderer, "resources/box.png", 320.0f, -80.0f); // Primera caja
    boxes.emplace_back(renderer, "resources/box.png", 200.0f, -50.0f); // Segunda caja


    unsigned int prev_ticks = SDL_GetTicks();
    Character duck;

    while (true) {
        unsigned int frame_ticks = SDL_GetTicks();
        unsigned int frame_delta = frame_ticks - prev_ticks;
        prev_ticks = frame_ticks;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                return 0;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    return 0;
            }
            StateManager::update_duck_state(duck, event);
        }

        duck.update_position(frame_delta, frame_ticks, renderer.GetOutputWidth());

        // Limpiar y dibujar el mapa
        renderer.Clear();
        mapa.dibujar_fondo(renderer); // Dibuja el fondo del mapa

        int src_x;
        int src_y;
        Coordinates duck_position = duck.get_coordinates();
        SDL_RendererFlip flip = duck.is_moving_to_the_right() ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

        if (duck.get_is_ducking()) {
            src_x = 0;
            src_y = 0;
            SDL_Rect dst_rect = { static_cast<int>(duck_position.pos_X),
                                  static_cast<int>(renderer.GetOutputHeight() / 2 - 32 - duck_position.pos_Y),
                                  64, 64 };

            SDL_RenderCopyEx(renderer.Get(), duckingSprites.Get(), nullptr, &dst_rect, 0.0, nullptr, flip);
        } else {
            src_x = DUCK_SPRITE_WIDTH * duck.get_movement_phase();
            src_y = DUCK_MOVEMENT_SPRITES_LINE;

            SDL_Rect src_rect = { src_x, src_y, DUCK_SPRITE_WIDTH, DUCK_SPRITE_HEIGHT };
            SDL_Rect dst_rect = { static_cast<int>(duck_position.pos_X),
                                  static_cast<int>(renderer.GetOutputHeight() / 2 - 32 - duck_position.pos_Y),
                                  64, 64 };
            SDL_RenderCopyEx(renderer.Get(), sprites.Get(), &src_rect, &dst_rect, 0.0, nullptr, flip);
        }
        
        
        //intento imprimir el arma
        //seteo la posicion del arma segun la orientación del pato
        float gun_position_x = duck.is_moving_to_the_right() ? duck_position.pos_X+16 : duck_position.pos_X;
        //ubicacion del sprite en el png y sus dimensiones
	SDL_Rect src_rect = { 1, 47, 32, 32 };
        SDL_Rect dst_rect = { static_cast<int>(gun_position_x),
                              static_cast<int>(renderer.GetOutputHeight() / 2 - 16 - duck_position.pos_Y),
                                  48, 48 };
            SDL_RenderCopyEx(renderer.Get(), pistolSprites.Get(), &src_rect, &dst_rect, 0.0, nullptr, flip);
            
            
        // Verificar colisión con cada caja de premios en el vector
        for (auto& box : boxes) {
            if (!box.is_collected() && duck.is_on_item(box, renderer) && duck.get_is_grabbing()) {
                box.set_collected(true);
            }
            // Renderizar la caja si no ha sido recogida
            if (!box.is_collected()) {
                box.render(renderer);
            }
        }



        renderer.Present();
        SDL_Delay(1);
    }
} catch (std::exception& e) {
    std::cerr << "Error in client main. " << e.what() << "\n";
    return -1;
}


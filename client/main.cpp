#include "common/foo.h"

#include <iostream>
#include <string>
#include <exception>

#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>

using namespace SDL2pp;
using namespace std;

int main() try {
    // Initialize SDL library
    SDL sdl(SDL_INIT_VIDEO);

    // Create main window: 640x480 dimensions, resizable, "SDL2pp demo" title
    Window window("SDL2pp demo",
                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                  640, 480,
                  SDL_WINDOW_RESIZABLE);

    // Create accelerated video renderer with default driver
    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    Texture background(renderer, std::string(DATA_PATH) + "/fondo.png");

    // Load the sprite sheet
    Texture sprites(renderer, std::string(DATA_PATH) + "/Duck.png");

    // Game state
    bool is_running = false; // whether the character is currently running
    int run_phase = -1;      // run animation phase
    float position = 0.0;    // player position
    string direction = "right"; //indica hacia donde está viendo el pato
    

    unsigned int prev_ticks = SDL_GetTicks();

    // Main loop
    while (1) {
        // Timing: calculate difference between this and previous frame in milliseconds
        unsigned int frame_ticks = SDL_GetTicks();
        unsigned int frame_delta = frame_ticks - prev_ticks;
        prev_ticks = frame_ticks;

        // Event processing:
        // - If window is closed, or Q or Escape buttons are pressed, quit the application
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 0;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: case SDLK_q:
                        return 0;
                    case SDLK_RIGHT: 
                        is_running = true;
                        direction = "right";
                        break;
                    case SDLK_LEFT: 
                    is_running = true; 
                    direction = "left";
                    break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_RIGHT: is_running = false; break;
                    case SDLK_LEFT: is_running = false; break;
                }
            }
        }

        // Update game state for this frame:
        // if character is runnung, move it to the right
        if (is_running) {
            if (direction == "right"){
                position += frame_delta * 0.2;
            } else if (direction == "left") {
                position -= frame_delta * 0.2;
            }
            
            run_phase = (frame_ticks / 100) % 6;
        } else {
            run_phase = 0;
        }

        // If player passes past the right side of the window, wrap him to the left side
        if (position > renderer.GetOutputWidth())
            position = -50;
        if (position < -50)
            position = renderer.GetOutputWidth();

        int vcenter = renderer.GetOutputHeight() / 2; // Y coordinate of window center

        // Clear screen
        renderer.Clear();

        renderer.Copy(background, Rect(0, 0, window.GetWidth(), window.GetHeight())); // Dibuja el fondo

        // Pick sprite from running animation sequence
        int sprite_width = 64;  // Ancho de cada imagen del pato caminando
        int sprite_high = 64;
        int src_x = sprite_width * run_phase;  // Se mueve al siguiente frame horizontalmente
        int src_y = 0;  // Y fijo en 0 si la animación está en la primera fila

        // Dibujar el sprite del pato caminando
        if (direction == "right") {
            renderer.Copy(
                    sprites,
                    Rect(src_x, src_y, sprite_width, sprite_high),  // Ajusta 32 si el alto de cada sprite es distinto
                    Rect((int)position, vcenter - 32, 64, 64)  // Escala de destino para que se vea más grande
            );
	} else if (direction == "left") {
	    renderer.Copy(
                    sprites,
                    Rect(src_x, src_y, sprite_width, sprite_high),  // Ajusta 32 si el alto de cada sprite es distinto
                    Rect((int)position, vcenter - 32, 64, 64),  // Escala de destino para que se vea más grande
                    0.0,
                    NullOpt,
                    SDL_FLIP_HORIZONTAL
            );
	}
        // Show rendered frame
        renderer.Present();

        // Frame limiter: sleep for a little bit to not eat 100% of CPU
        SDL_Delay(1);
    }



    return 0;
} catch (std::exception& e) {
    // If case of error, print it and exit with error
    std::cerr << e.what() << std::endl;
    return 1;
}


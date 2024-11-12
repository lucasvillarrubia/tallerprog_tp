#include <exception>
#include <iostream>
#include <SDL.h>

#include "client.h"
#include "SDL2pp/Renderer.hh"
#include "SDL2pp/SDL.hh"
#include "SDL2pp/Window.hh"


const int REQUIRED_ARGS_QTY = 3;
const int SUCCESSFUL_RUN = 0;
const int PROGRAM_CALL_ERROR = -1;
const int CAUGHT_ERROR = -1;
const int UNKNOWN_ERROR = -1;


int main(int argc, char* argv[]) {
    try {
        const char* hostname = nullptr;
        const char* servname = nullptr;
        if (argc != REQUIRED_ARGS_QTY) {
            std::cerr << "Bad Client program call. Expected: ./client hostname portname"
                      << "\n";
            return PROGRAM_CALL_ERROR;
        }
        hostname = argv[1];
        servname = argv[2];
        SDL2pp::SDL sdl(SDL_INIT_VIDEO);
        Client client(hostname, servname);
        client.run();
        return SUCCESSFUL_RUN;
    } catch (const std::exception& error) {
        std::cerr << "Thrown exception on client's side. Cause: " << error.what() << "\n";
        return CAUGHT_ERROR;
    } catch (...) {
        std::cerr << "Unknown exception on client's side.\n";
        return UNKNOWN_ERROR;
    }
}
#include "client.h"

#include <iostream>
#include <SDL.h>
#include <sstream>
#include <utility>

#include "SDL2pp/SDL.hh"


Client::Client(const char* hostname, const char* servname):
        window("Duck Game",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            640, 480,
            SDL_WINDOW_RESIZABLE),
        renderer(window, -1, SDL_RENDERER_ACCELERATED),
        connected(false),
        game_on(false),
        connection(std::move(Socket(hostname, servname)), events, updates, connected),
        event_listener(game_on, events),
        renderloop(game_on, window, renderer, updates, state),
        updater(updates, state) {}

void Client::run() {
    try
    {
        // SDL2pp::Window window(
        //     "Duck Game",
        //     SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        //     640, 480,
        //     SDL_WINDOW_RESIZABLE
        // );
        // SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);
        game_on.store(true);
        connection.start_communication();
        // updater.start();
        // pantalla de inicio
        // preguntar para 1 o 2 jugadores -> sólo debería activar las teclas para el jugador 2 y un
        while (game_on.load() && connected.load())
        {
            event_listener.run();
            if (not game_on.load()) break;
            renderloop.run();
        }
        connection.end_connection();
        updates.close();
        // updater.stop();
        // updater.join();
        events.close();
    }
    catch (ClosedQueue const& e)
    {
        std::cerr << "Se cerró la queue del cliente?! " << e.what() << '\n';
        connection.end_connection();
        updates.close();
        // updater.stop();
        // updater.join();
        events.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception thrown on a client's side: " << e.what() << '\n';
        connection.end_connection();
        updates.close();
        // updater.stop();
        // updater.join();
        events.close();
    }
    catch (...)
    {
        std::cerr << "Unknown exception on a client's side." << '\n';
        connection.end_connection();
        updates.close();
        // updater.stop();
        // updater.join();
        events.close();
    }
}

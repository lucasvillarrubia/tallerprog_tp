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

void Client::constant_rate_loop(std::function<void(int)> processing, std::chrono::milliseconds rate)
{
    auto t1 = std::chrono::steady_clock::now();
    int it = 0;
    while (true)
    {
        processing(it);
        if (not game_on.load() or not connected.load()) break;
        auto t2 = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        auto rest = rate - elapsed;
        if (rest.count() < 0)
        {
            auto behind = -rest;
            auto lost = behind - behind % rate;
            t1 += lost;
            it += static_cast<int>(lost.count() / rate.count());
        }
        else
        {
            std::this_thread::sleep_for(rate);
        }
        t1 += rate;
        ++it;
    }
}

void Client::run() {
    try
    {
        std::chrono::milliseconds rate(16);
        game_on.store(true);
        connection.start_communication();
        // updater.start();
        // pantalla de inicio
        // preguntar para 1 o 2 jugadores -> sólo debería activar las teclas para el jugador 2 y un
        while (game_on.load() && connected.load())
        {
            constant_rate_loop([&](int frame)
            {
                event_listener.run();
                if (not game_on.load()) return;
                renderloop.run(frame);
            }, rate);
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

#include "client.h"

#include <iostream>
#include <SDL.h>
#include <sstream>
#include <utility>

#include "SDL2pp/SDL.hh"
#include <QApplication>
#include "lobby/lobby.h"




Client::Client(const char* hostname, const char* servname):
        app(argc, argv),
        // window("Duck Game",
        //     SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        //     640, 480,
        //     SDL_WINDOW_RESIZABLE),
        // renderer(window, -1, SDL_RENDERER_ACCELERATED),
        connected(false),
        game_on(false),
        connection(std::move(Socket(hostname, servname)), events, updates, connected),
        event_listener(game_on, events),
        // renderloop(game_on, updates, state),
        // renderloop(game_on, window, renderer, updates, state),
        updater(updates, state) {
            connect(&gamelobby, &lobby::create_one_player_match, this, &Client::handle_create_one_player_match);
        }

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
        // int argc = 0;
        // char** argv = nullptr;
        // QApplication app(argc, argv);
        // lobby lobby;


        // botón de refresh y NO actualizaciones en vivo de partidas disponibles
        gamelobby.show();
        app.exec();
        SDL2pp::SDL sdl(SDL_INIT_VIDEO);
        Renderer renderloop(game_on, updates, state);
        // updater.start();
        // pantalla de inicio
        // preguntar para 1 o 2 jugadores -> sólo debería activar las teclas para el jugador 2 y un
        // Gameaction create(1, 0, 4, 0);
        // events.try_push(create);
        // Gameaction start(1, 1, 6, 0);
        // events.try_push(start);
        while (game_on.load() && connected.load())
        {
            constant_rate_loop([&](int frame)
            {
                event_listener.run();
                renderloop.render(frame);
            }, rate);
        }
        updates.close();
        events.close();
        connection.end_connection();
        // updater.stop();
        // updater.join();
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

void Client::handle_create_one_player_match()
{
    std::cout << "create one player match\n";
    Gameaction create(1, 0, 4, 0);
    events.try_push(create);
}

void Client::handle_create_two_player_match()
{
    Gameaction create(1, 0, 7, 0);
    events.try_push(create);
}

void Client::handle_create_three_player_match()
{
    Gameaction create(1, 0, 8, 0);
    events.try_push(create);
}

void Client::handle_join_match()
{
    Gameaction start(1, 1, 5, 0);
    events.try_push(start);
}

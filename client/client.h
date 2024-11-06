#ifndef CLIENT_H
#define CLIENT_H


#include "client_threads/event_listener.h"
#include "local_player.h"
#include "client_threads/renderer.h"
#include "common/gamedata.h"
#include "SDL2pp/Renderer.hh"
#include "SDL2pp/Window.hh"


class Client
{
private:
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;
    std::atomic_bool connected;
    std::atomic_bool game_on;
    Queue<Gameaction> events;
    Queue<Gamestate> updates;
    LocalPlayer connection;
    EventListener eventloop;
    Renderer renderloop;
public:
    Client(const char*, const char*);
    void run();
    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;
    Client(Client&& other) = delete;
    Client& operator=(Client&& other) = delete;
    ~Client() = default;
};


#endif

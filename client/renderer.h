#ifndef RENDERER_H
#define RENDERER_H


#include "client/character.h"
#include "common/gamedata.h"
#include "common/hands_on_threads/queue.h"
#include "common/hands_on_threads/thread.h"
#include "SDL2pp/Renderer.hh"
#include "SDL2pp/Window.hh"
#include "state_manager.h"


class Renderer
{
private:
    std::atomic_bool& connected;
    SDL2pp::Window& window;
    SDL2pp::Renderer& renderer;
    Queue<Gamestate>& updates_feed;
    StateManager& state;
    // Character duck;
public:
    Renderer(std::atomic_bool&, SDL2pp::Window&, SDL2pp::Renderer&, Queue<Gamestate>&, StateManager&);
    void run();
    ~Renderer() = default;
};


#endif

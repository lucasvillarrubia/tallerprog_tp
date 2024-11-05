#ifndef RENDERER_H
#define RENDERER_H


#include "common/gamedata.h"
#include "common/hands_on_threads/queue.h"
#include "common/hands_on_threads/thread.h"
#include "SDL2pp/Renderer.hh"
#include "SDL2pp/Window.hh"


class Renderer
{
private:
    std::atomic_bool& connected;
    Queue<Gamestate>& updates_feed;
    // std::mutex mtx;
public:
    Renderer(std::atomic_bool&, Queue<Gamestate>&);
    void run();
    ~Renderer() = default;
};


#endif

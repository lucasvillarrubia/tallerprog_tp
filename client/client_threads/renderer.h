#ifndef RENDERER_H
#define RENDERER_H


#include "common/gamestate.h"
#include "common/hands_on_threads/queue.h"
#include "common/hands_on_threads/thread.h"


class Renderer: public Thread
{
private:
    Queue<Gamestate>& updates_feed;
public:
    explicit Renderer(Queue<Gamestate>&);
    void run() override;
    ~Renderer() override = default;
};


#endif

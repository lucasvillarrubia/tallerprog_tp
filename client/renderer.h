#ifndef RENDERER_H
#define RENDERER_H


#include "common/gamestate.h"
#include "common/queue.h"
#include "common/thread.h"


class Renderer: public Thread
{
private:
    Queue<Gamestate>& updates_feed;
public:
    explicit Renderer(Queue<Gamestate>&);
};


#endif

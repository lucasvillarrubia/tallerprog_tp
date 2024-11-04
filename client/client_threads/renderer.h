#ifndef RENDERER_H
#define RENDERER_H


#include "common/gamedata.h"
#include "common/hands_on_threads/queue.h"
#include "common/hands_on_threads/thread.h"


class Renderer: public Thread
{
private:
    std::atomic_bool is_running;
    Queue<Gamestate>& updates_feed;
public:
    explicit Renderer(Queue<Gamestate>&);
    void run() override;
    void stop() override;
    ~Renderer() override = default;
};


#endif

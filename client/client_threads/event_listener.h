#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H


#include "common/gameaction.h"
#include "common/hands_on_threads/queue.h"
#include "common/hands_on_threads/thread.h"


class EventListener: public Thread
{
private:
    Queue<Gameaction>& inputs;
public:
    EventListener(Queue<Gameaction>&);
    void run() override;
};


#endif

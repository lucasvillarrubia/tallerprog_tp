#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H


#include "common/gameaction.h"
#include "common/queue.h"
#include "common/thread.h"


class EventListener: public Thread
{
private:
    Queue<Gameaction>& inputs;
public:
    EventListener(Queue<Gameaction>&);
    void run() override;
};


#endif

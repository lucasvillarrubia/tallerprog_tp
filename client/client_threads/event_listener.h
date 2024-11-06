#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H


#include <map>
#include <SDL_events.h>
#include <SDL_keycode.h>

#include "common/gamedata.h"
#include "common/hands_on_threads/queue.h"
#include "common/hands_on_threads/thread.h"


class EventListener
{
private:
    std::atomic_bool& connected;
    Queue<Gameaction>& events;
    // Por lo que leí también se puede usar SDL_EventFilter
    std::map<Uint32, int> codes_by_event_type = {
        {SDL_QUIT, 1},
        {SDL_KEYDOWN, 2},
        {SDL_KEYUP, 3}
    };
    std::map<SDL_Keycode, int> codes_by_key = {
        {SDLK_ESCAPE, 9},
        {SDLK_RIGHT, 1},
        {SDLK_LEFT, 2},
        {SDLK_SPACE, 3}
    };
public:
    EventListener(std::atomic_bool&, Queue<Gameaction>&);
    void run();
    void stop();
    ~EventListener() = default;
};


#endif

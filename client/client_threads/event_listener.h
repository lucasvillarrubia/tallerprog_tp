#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include <atomic>
#include <map>
#include <SDL2/SDL_events.h>
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
        {SDL_QUIT, 9},
        {SDL_KEYDOWN, 2},
        {SDL_KEYUP, 3}
        // 4 para crear partida
        // 5 para unirse a partida
        // 6 para comenzar partida
    };
    std::map<SDL_Keycode, int> codes_by_key = {
        {SDLK_ESCAPE, 9},
        {SDLK_RIGHT, 1},
        {SDLK_LEFT, 2},

        {SDLK_SPACE, 3},

        // {SDLK_RETURN, 3}
        {SDLK_g, 4},
        {SDLK_f, 5}

    };
    std::map<SDL_Keycode, int> codes_by_key_for_second_player = {
        {SDLK_ESCAPE, 9},
        {SDLK_d, 1},
        {SDLK_a, 2},
        {SDLK_RETURN, 3}
    };
    const bool& is_multiplayer;
    const int& match_id;
public:

    EventListener(std::atomic_bool&, Queue<Gameaction>&, const bool&, const int&);
    void listen();

    ~EventListener() = default;
};


#endif

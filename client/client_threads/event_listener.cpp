#include "event_listener.h"

#include <SDL_events.h>


EventListener::EventListener(std::atomic_bool& connection, Queue<Gameaction>& q):
        connected(connection),
        events(q) {}

void EventListener::run() {
    try
    {
        SDL_Event event;
        if (SDL_PollEvent(&event) && connected.load())
        {
            Uint32 type = event.type;
            SDL_Keycode key = event.key.keysym.sym;
            if (not codes_by_event_type.contains(type) || not codes_by_key.contains(key))
                return;
            const int type_code = codes_by_event_type.at(event.type);
            const int key_code = codes_by_key.at(event.key.keysym.sym);
            Gameaction new_action(1, 1, type_code, key_code);
            events.try_push(new_action);
            if(type == SDL_QUIT || key == SDLK_ESCAPE)
                connected.store(false);
        }
    }
    catch (ClosedQueue const& e)
    {
        std::cerr << "Se cerró la queue del juego?! " << e.what() << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught in the renderer thread: " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Unknown exception on the renderloop.\n";
    }
}

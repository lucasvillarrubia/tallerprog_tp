#include "event_listener.h"

#include <SDL_events.h>


EventListener::EventListener(std::atomic_bool& connection, std::condition_variable& con_warner, Queue<Gameaction>& q):
        is_running(false),
        game_on(connection),
        connection_ended(con_warner),
        events(q) {}

void EventListener::run() {
    try
    {
        is_running.store(true);
        SDL_Event event;
        while (SDL_WaitEvent(&event) && is_running.load())
        {
            Uint32 type = event.type;
            SDL_Keycode key = event.key.keysym.sym;
            if (not codes_by_event_type.contains(type) || not codes_by_key.contains(key))
                continue;
            const int type_code = codes_by_event_type.at(event.type);
            const int key_code = codes_by_key.at(event.key.keysym.sym);
            Gameaction new_action(1, type_code, key_code);
            if (events.try_push(new_action))
                std::cout << "Evento ya está en la queue para mandar! " << type_code << " " << key_code << "\n";
            if(type == SDL_QUIT || key == SDLK_ESCAPE) {
                game_on.store(false);
                connection_ended.notify_all();
                is_running.store(false);
            }
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

void EventListener::stop() {
    _keep_running = false;
    is_running.store(false);
}

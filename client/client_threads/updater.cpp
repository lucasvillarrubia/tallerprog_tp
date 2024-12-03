#include "updater.h"

#include "client/state_manager.h"


const int ENDGAME = 14;


Updater::Updater(Queue<Gamestate>& q, StateManager& s): is_running(false), updates_feed(q), state(s) {}

void Updater::run()
{
    try
    {
        is_running.store(true);
        while (is_running.load())
        {
            Gamestate update = updates_feed.pop();
            if (update.type == ENDGAME)
            {
                stop();
                std::cout << "Game ended\n";
                break;
            }
            if (update.type == 13)
            {
                std::cout << "Round advanced\n";
                state.reset();
                continue;
            }
            state.update(update);
            while (updates_feed.try_pop(update))
            {
                state.update(update);
            }
        }
    }
    catch (ClosedQueue const& e)
    {
        std::cerr << "Se cerró la queue del updater?! " << e.what() << '\n';
        stop();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught in the updater thread: " << e.what() << '\n';
        stop();
    }
    catch (...)
    {
        std::cerr << "Unknown exception on the updateloop.\n";
        stop();
    }
}

void Updater::stop()
{
    _keep_running = false;
    is_running.store(false);
}

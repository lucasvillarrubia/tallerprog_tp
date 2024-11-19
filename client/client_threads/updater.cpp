#include "updater.h"

#include "client/state_manager.h"

Updater::Updater(Queue<Gamestate>& q, StateManager& s): is_running(false), updates_feed(q), state(s) {}

void Updater::run()
{
    try
    {
        is_running.store(true);
        while (is_running.load())
        {
            Gamestate update = updates_feed.pop();
            state.update(update);
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
    }
    catch (...)
    {
        std::cerr << "Unknown exception on the updateloop.\n";
    }
}

void Updater::stop()
{
    _keep_running = false;
    is_running.store(false);
}

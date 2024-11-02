#include "cleaner.h"

Cleaner::Cleaner(MonitoredList& player_list): is_running(false), players(player_list) {}

void Cleaner::run() {
    try
    {
        is_running.store(true);
        while (is_running.load()) {
            // capaz estaría bueno agregar un sleep acá
            // si no, ver la manera de implementar un wait acá y que en el gameloop se notifique
            // cuando se desconecte un jugador (y se broadcastee ese estado a los jugadores)
            // la std::condition_variable la tendría Server (main thread), Cleaner y Gameloop sólo refs
            players.clean_up();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception thrown on the cleaner thread: " << e.what() << '\n';
        is_running.store(false);
    }
    catch (...)
    {
        std::cerr << "Unknown exception on the cleaner.\n";
        is_running.store(false);
    }
}

void Cleaner::stop()
{
    _keep_running = false;
    is_running.store(false);
    // acá se va a recorrer la lista de partidas para sacar a los jugadores de cada una
    // Server sólo haría clear() de la lista de partidas
}

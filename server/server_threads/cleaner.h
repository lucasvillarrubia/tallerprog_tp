#ifndef CLEANER_H
#define CLEANER_H


#include "common/hands_on_threads/thread.h"
#include "server/monitored_list.h"
#include "server/match.h"


class Cleaner: public Thread
{
private:
    // proximamente va a
        // recorrer todas las partidas
        // limpiar los jugadores desconectados de cada partida
        // limpiar las partidas que hayan terminado!
    // MonitoredList<Match>& matches;
    std::atomic_bool is_running;
    MonitoredList<Match*>& matches;
public:
    explicit Cleaner(MonitoredList<Match*>&);
    void run() override;
    void stop() override;
    ~Cleaner() override = default;
};


#endif //CLEANER_H

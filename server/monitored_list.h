#ifndef MONITORED_LIST_H
#define MONITORED_LIST_H


#include <list>
#include <mutex>

#include "command.h"
#include "player.h"


// Clase monitor de lista con su propio mutex y la lista a proteger.
// RAII: no permite copia.

// Nota: nombre polémico para una lista de punteros a jugadores
//      pero no me quedó tiempo para agregarle el template<>
//      (aunque el clean_up es un método raro para una lista cualquiera,
//      hubiera agregado que reciba un predicado y se renombraba a remove_all_that)


class MonitoredList {
private:
    std::list<Player*> list;
    std::mutex mtx;

public:
    MonitoredList();
    void push_back(Player*);
    void broadcast(const Command&);
    void clean_up();
    void clear();
    MonitoredList(const MonitoredList&) = delete;
    MonitoredList& operator=(const MonitoredList&) = delete;
    ~MonitoredList();
};


#endif
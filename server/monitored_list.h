#ifndef MONITORED_LIST_H
#define MONITORED_LIST_H


#include <list>
#include <mutex>

#include "command.h"
#include "player.h"


// coming soon acá:
    // broadcast    ---> for_each   (*función*)
    // clean_up     ---> remove_if  (*predicado*)
// dejando clear como está, se tiene que aclarar que la lista sólo va a servir con punteros
// (de cualquier cosa reservada en el heap, no se puede hacer un delete de cualquier cosa)


//template <Typename T>
class MonitoredList {
private:
    // std::list<T> list;
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
    ~MonitoredList() = default;
};


#endif
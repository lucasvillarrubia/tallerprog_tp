#ifndef MONITORED_LIST_H
#define MONITORED_LIST_H


#include <functional>
#include <list>
#include <mutex>

#include "command.h"
#include "player.h"


// coming soon acá:
    // broadcast    ---> for_each   (*función*)
    // clean_up     ---> remove_if  (*predicado*)
// dejando clear como está, se tiene que aclarar que la lista sólo va a servir con punteros
// (de cualquier cosa reservada en el heap, no se puede hacer un delete de cualquier cosa)


template <typename T>
class MonitoredList {
private:
    std::list<T> list;
    //std::list<Player*> list;
    std::mutex mtx;

public:
    //MonitoredList();
    //void push_back(T);
    // void push_back(Player*);
    //void broadcast(const Gamestate&);
    // le tiene que llegar un predicado
    //MonitoredList<T> select_if();
    //void clean_up();
    //void clear();
    // MonitoredList(const MonitoredList&) = delete;
    // MonitoredList& operator=(const MonitoredList&) = delete;
    // ~MonitoredList() = default;





    MonitoredList() = default;

    void push_back(T player) {
        std::unique_lock<std::mutex> lck(mtx);
        list.push_back(player);
    }

    void for_each(std::function<void(T&)> func) {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto& gameobject : list) {
            func(gameobject);
        }
    }

    T get_by_id(int id) {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto& gameobject : list) {
            if (gameobject.matches(id)) {
                return gameobject;
            }
        }
        return nullptr;
    }

    void broadcast(const Gamestate& announce) {
        std::unique_lock<std::mutex> lck(mtx);
        for (auto& player: list) {
            player->add_message_to_queue(announce);
        }
    }

    MonitoredList select_if(std::function<bool(const T&)> predicate) {
        MonitoredList<T> result;
        std::lock_guard<std::mutex> lock(mtx);
        for (const auto& item : list) {
            if (predicate(item)) {
                result.push_back(item);
            }
        }
        return result;
    }

    void clean_up() {
        std::unique_lock<std::mutex> lck(mtx);
        list.remove_if([](auto player) {
            if (not player->is_connected()) {
                player->disconnect();
                delete player;
                return true;
            }
            return false;
        });
    }

    void clear() {
        std::unique_lock<std::mutex> lck(mtx);
        for (auto player: list) {
            player->disconnect();
            delete player;
        }
    }

    int size()
    {
        return list.size();
    }

    bool contains(int player)
    {
        for (auto gameobject: list) {
            if (gameobject.matches(player))
                return true;
        }
        return false;
    }

    MonitoredList(const MonitoredList&) = delete;
    MonitoredList& operator=(const MonitoredList&) = delete;
    ~MonitoredList() = default;
};


#endif
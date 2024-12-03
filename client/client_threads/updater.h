#ifndef UPDATER_H
#define UPDATER_H


#include "client/state_manager.h"
#include "common/gamedata.h"
#include "common/hands_on_threads/queue.h"
#include "common/hands_on_threads/thread.h"


class Updater: public Thread {
private:
    std::atomic_bool is_running;
    Queue<Gamestate>& updates_feed;
    StateManager& state;
public:
    Updater(Queue<Gamestate>&, StateManager&);
    void run() override;
    void stop() override;
    ~Updater() override = default;
};


#endif

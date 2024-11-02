#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <atomic>

#include "common/hands_on_threads/queue.h"
#include "common/hands_on_threads/thread.h"
#include "server/command.h"
#include "server/monitored_list.h"


class Gameplay: public Thread
{
private:
    std::atomic_bool is_running;
    MonitoredList& players;
    Queue<Command>& user_commands;
    void process_users_commands();
public:
    Gameplay(MonitoredList&, Queue<Command>&);
    void run() override;
    void stop() override;
    ~Gameplay() override = default;
};


#endif
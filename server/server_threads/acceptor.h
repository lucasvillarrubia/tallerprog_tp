#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <atomic>

#include "common/hands_on_sockets/socket.h"
#include "common/hands_on_threads/queue.h"
#include "common/hands_on_threads/thread.h"
#include "server/command.h"
#include "server/monitored_list.h"


class Acceptor: public Thread
{
private:
    std::atomic_bool is_running;
    Socket acceptor;
    MonitoredList& player_list;
    Queue<Command>& user_commands;

public:
    Acceptor(const char*, MonitoredList&, Queue<Command>&);
    void run() override;
    void stop() override;
    ~Acceptor() = default;
};


#endif
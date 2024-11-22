#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <atomic>

#include "common/hands_on_sockets/socket.h"
#include "common/hands_on_threads/queue.h"
#include "common/hands_on_threads/thread.h"
#include "server/monitored_list.h"
#include "server/player.h"
#include "common/gamedata.h"


class Acceptor: public Thread
{
private:
    std::atomic_bool is_running;
    Socket acceptor;
    MonitoredList<Player*>& player_list;
    Queue<Gameaction>& user_commands;
    int player_count;
public:
    Acceptor(const char*, MonitoredList<Player*>&, Queue<Gameaction>&);
    void run() override;
    void stop() override;
    ~Acceptor() = default;
};


#endif
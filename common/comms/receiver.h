#ifndef RECEIVER_H
#define RECEIVER_H


#include <atomic>

#include "common/hands_on_threads/thread.h"


// Objeto activo que encapsula un hilo recibidor.


class Receiver: public Thread
{
protected:
    std::atomic_bool is_running;
    std::atomic_bool& client_disconnected;
    virtual void receive_data() = 0;
public:
    explicit Receiver(std::atomic_bool&);
    void run() override;
    void stop() override;
    ~Receiver() override = default;
};


#endif

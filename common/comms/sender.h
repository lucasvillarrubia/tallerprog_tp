#ifndef SENDER_H
#define SENDER_H


#include <atomic>

#include "common/hands_on_threads/thread.h"


// Objeto activo que encapsula un hilo enviador.

// preguntar después si conviene hacerla template, junto con Receiver
// porque sí o sí estos hilos en esta estructura van a trabajar con queues
// (en los run tengo un catch esperando una posible ClosedQueue y
// en sender.cpp incluyo queue.h pero sólo por la excepción ClosedQueue)
// entonces un posible atributo sería:
// Queue<T>& source;


class Sender: public Thread
{
protected:
    std::atomic_bool is_running;
    std::atomic_bool& client_is_connected;
    virtual void send_data() = 0;
public:
    explicit Sender(std::atomic_bool&);
    void run() override;
    void stop() override;
    ~Sender() override = default;
};


#endif

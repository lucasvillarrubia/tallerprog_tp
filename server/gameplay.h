#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <atomic>
#include <map>

#include "../common/queue.h"
#include "../common/thread.h"

#include "command.h"
#include "monitored_list.h"


const int DEFAULT_RESPAWN_ITERATIONS_BOX_1 = 15;
const int DEFAULT_RESPAWN_ITERATIONS_BOX_2 = 5;
const int DEFAULT_RESPAWN_ITERATIONS_BOX_3 = 20;
const int DEFAULT_RESPAWN_ITERATIONS_BOX_4 = 15;


// Objeto activo que representa al gameloop.
// (Próximo a implementar) En este hilo está el único sleep del tp.
//
// Comparte:
// - con el servidor, la queue de las acciones de los usuarios (mensajes ya deserializados).
// - con el Acceptor, la lista de jugadores.
// RAII: hereda de Thread (no permite copia ni movimiento).


class Gameplay: public Thread {
private:
    std::atomic_bool is_running;
    MonitoredList& players;
    Queue<Command>& user_commands;
    std::map<int, int> rewards_by_box = {{0x01, 0x10}, {0x02, 0x11}, {0x03, 0x12}, {0x04, 0x13}};
    std::map<int, int> default_respawn_iterations_by_box = {
            {0x01, DEFAULT_RESPAWN_ITERATIONS_BOX_1},
            {0x02, DEFAULT_RESPAWN_ITERATIONS_BOX_2},
            {0x03, DEFAULT_RESPAWN_ITERATIONS_BOX_3},
            {0x04, DEFAULT_RESPAWN_ITERATIONS_BOX_4}};
    std::map<int, int> iterations_left_by_dead_box;
    std::map<int, bool> boxes;
    bool is_box_available(int);
    void process_users_commands();
    void check_for_boxes_respawns();

public:
    Gameplay(MonitoredList&, Queue<Command>&);
    void run() override;
    void stop() override;
    ~Gameplay();
};


#endif
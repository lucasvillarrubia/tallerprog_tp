#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <atomic>
#include <cstdint>
#include <mutex>
#include <vector>

#include "../common/socket.h"

#include "command.h"


// Clase de protocolo de servidor. Pensada para ser de un único cliente.
// Maneja (y posee) el Socket en el que se comparten los enviós
// y recibimientos (con los dos streams que tiene el Socket).
// Es compartido por el Sender y el Receiver!
// RAII: no permite copia.


class ServerProtocol {
private:
    Socket peer;
    std::mutex mtx;
    std::atomic_bool& client_disconnected;
    void receive_single_int_message(uint8_t&);
    void receive_string_message(std::vector<char>&);
    void send_single_int_message(uint8_t);
    void send_string_message(const std::vector<char>&);

public:
    ServerProtocol(Socket&&, std::atomic_bool&);
    Command receive_message();
    void send_pickup_message(int, const std::vector<char>&);
    void send_box_appearence_message();
    void close_comms();
    ServerProtocol(const ServerProtocol&) = delete;
    ServerProtocol& operator=(const ServerProtocol&) = delete;
    ~ServerProtocol();
};


#endif
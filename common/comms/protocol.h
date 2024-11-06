#ifndef PROTOCOL_H
#define PROTOCOL_H


#include <atomic>
#include <cstdint>
#include <mutex>
#include <vector>

#include "common/gamedata.h"
#include "common/hands_on_sockets/socket.h"


// Clase abstracta protocolo que ofrece a sus clases derivadas envío y recibimiento de mensajes de tipo string y enteros de 8 bytes.


class Protocol
{
protected:
    Socket peer;
    std::mutex mtx;
    std::atomic_bool& client_disconnected;
    void receive_single_int_message(uint8_t&);
    void receive_string_message(std::vector<char>&);
    void send_single_int_message(uint8_t);
    void send_string_message(const std::vector<char>&);
public:
    Protocol(Socket&&, std::atomic_bool&);
    virtual void send_message(const Gamedata&) = 0;
    virtual Gamedata receive_message() = 0;
    void close_comms();
    Protocol(const Protocol&) = delete;
    Protocol& operator=(const Protocol&) = delete;
    virtual ~Protocol() = default;
};


#endif

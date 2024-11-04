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
    std::atomic_bool& client_is_connected;
    void receive_single_int(uint8_t&);
    void receive_string(std::vector<char>&);
    void send_single_int(uint8_t);
    void send_string(const std::vector<char>&);
    void receive_single_float(float&);
    void send_single_float(float);
public:
    Protocol(Socket&&, std::atomic_bool&);
    void close_comms();
    Protocol(const Protocol&) = delete;
    Protocol& operator=(const Protocol&) = delete;
    virtual ~Protocol() = default;
};


#endif

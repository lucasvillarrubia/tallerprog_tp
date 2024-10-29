#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H


#include <cstdint>
#include <vector>

#include "../common/message.h"
#include "../common/socket.h"


// Clase protocolo de cliente, en su mayoría reutilizada del tp de Sockets.
// Envía el único tipo de mensaje estipulado en el enunciado.
// Recibe los dos tipos de mensaje estipulados en el enunciado.


class ClientProtocol {
private:
    Socket peer;
    void receive_single_int_message(uint8_t&);
    void receive_string_message(std::vector<char>&);
    void send_single_int_message(uint8_t);
    void send_string_message(const std::vector<char>&);
    bool theres_more_data_per_code(int);

public:
    explicit ClientProtocol(Socket&&);
    ClientProtocol(const ClientProtocol&) = delete;
    ClientProtocol& operator=(const ClientProtocol&) = delete;
    ~ClientProtocol();
};


#endif
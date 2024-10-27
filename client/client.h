#ifndef CLIENT_H
#define CLIENT_H


#include <string>
#include <vector>

#include "../common/socket.h"

#include "client_protocol.h"


// Clase de entidad de cliente. Sólo trabaja con un hilo (el main).
// Ejecuta acciones del cliente según lo estipulado por el enunciado.
// Imprime los mensajes recibidos desde el servidor y recibe los comandos del usuario por entrada
// estándar.
// RAII: no permite copia ni movimiento.

// Nota: No llegué a hacer una clase Parser para la entrada del usuario, ni verificaciones.
//      La excepción que planeaba "BadInput" se debería haber lanzado en el método
//      perform_actions_by_command.


class Client {
private:
    Socket client;
    ClientProtocol protocol;
    std::vector<char> player_name;
    void print_ask_for_player_name();
    void get_user_player_name();
    std::string get_command_input();
    void perform_actions_by_command(const std::string&);
    void print_received_messages(const std::string&);
    void send_pickup_message(const std::string&);

public:
    explicit Client(const char*, const char*);
    void run();
    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;
    Client(Client&& other) = delete;
    Client& operator=(Client&& other) = delete;
    ~Client();
};


#endif
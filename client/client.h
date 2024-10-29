#ifndef CLIENT_H
#define CLIENT_H


#include <string>
#include <vector>

#include "client_protocol.h"


// Clase de entidad de cliente.
// RAII: no permite copia ni movimiento.


class Client {
private:
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
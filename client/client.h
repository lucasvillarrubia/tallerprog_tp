#ifndef CLIENT_H
#define CLIENT_H


#include <string>
#include <vector>

#include "client_comms/client_protocol.h"
#include "client_threads/event_listener.h"
#include "local_player.h"
#include "client_threads/renderer.h"


// Clase de entidad de cliente.
// RAII: no permite copia ni movimiento.


class Client {
private:
    // ClientProtocol protocol;
    Queue<Gameaction> events;
    Queue<Gamestate> updates;
    // LocalPlayer player_comms;
    // std::vector<char> player_name;
    Renderer renderloop;
    EventListener eventloop;
    std::string get_command_input();

public:
    // explicit Client(const char*, const char*);
    explicit Client();
    void run();
    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;
    Client(Client&& other) = delete;
    Client& operator=(Client&& other) = delete;
    ~Client();
};


#endif
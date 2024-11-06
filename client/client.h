#ifndef CLIENT_H
#define CLIENT_H


#include "client_threads/event_listener.h"
#include "local_player.h"
#include "client_threads/renderer.h"
#include "common/gamedata.h"


class Client
{
private:
    std::atomic_bool connected;
    std::condition_variable connection_ended;
    std::mutex mtx;
    Queue<Gameaction> events;
    Queue<Gamestate> updates;
    LocalPlayer connection;
    EventListener eventloop;
    Renderer renderloop;
    // std::string get_command_input();
public:
    Client(const char*, const char*);
    void run();
    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;
    Client(Client&& other) = delete;
    Client& operator=(Client&& other) = delete;
    ~Client() = default;
};


#endif

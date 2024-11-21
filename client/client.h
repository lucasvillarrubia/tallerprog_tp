#ifndef CLIENT_H
#define CLIENT_H


#include "client_threads/event_listener.h"
#include "local_player.h"
#include "renderer.h"
#include "state_manager.h"
#include "client_threads/updater.h"
#include "common/gamedata.h"
#include "SDL2pp/Renderer.hh"
#include "SDL2pp/Window.hh"
#include "lobby/lobby.h"
#include <QApplication>
#include <QObject>




class Client: public QObject
{
    Q_OBJECT
private:
    // SDL2pp::Window window;
    // SDL2pp::Renderer renderer;
    int argc = 0;
    char** argv = nullptr;
    QApplication app;
    std::atomic_bool connected;
    std::atomic_bool game_on;
    Queue<Gameaction> events;
    Queue<Gamestate> updates;
    LocalPlayer connection;
    EventListener event_listener;
    StateManager state;
    // Renderer renderloop;
    Updater updater;
    lobby gamelobby;
    bool multiplayer_mode;
    void constant_rate_loop(std::function<void(int)>, std::chrono::milliseconds);
public:
    Client(const char*, const char*);
    void run();
    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;
    Client(Client&& other) = delete;
    Client& operator=(Client&& other) = delete;
    ~Client() = default;
public slots:
    void handle_create_one_player_match();
    void handle_create_two_player_match();
    void handle_create_three_player_match();
    void handle_join_match();
};


#endif

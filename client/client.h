#ifndef CLIENT_H
#define CLIENT_H


#include "client_threads/event_listener.h"
#include "local_player.h"
#include <SDL2/SDL_mixer.h>
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
    SDL2pp::SDL sdl;
    int current_player_count;
    Renderer renderloop;
    Updater updater;
    lobby gamelobby;
    Mix_Music* background_music;
    bool multiplayer_mode;
    int current_id;
    int current_match;
    int current_match_winner;
    void constant_rate_loop(std::function<void(int)>, std::chrono::milliseconds);
    void show_winner_message();
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
    void handle_join_match(int);
    void handle_start_match();
    void handle_refresh_lobby();
};


#endif

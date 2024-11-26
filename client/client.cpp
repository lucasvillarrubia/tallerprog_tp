#include "client.h"

#include <iostream>
#include <SDL.h>
#include <sstream>
#include <utility>

#include "SDL2pp/SDL.hh"
#include <QApplication>
#include "lobby/lobby.h"
#include <QMessageBox>
#include <QIcon>
#include <QAbstractButton>
#include <QString>


const int MULTIPLAYER_MODE = 0;


Client::Client(const char* hostname, const char* servname):
        app(argc, argv),
        // window("Duck Game",
        //     SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        //     640, 480,
        //     SDL_WINDOW_RESIZABLE),
        // renderer(window, -1, SDL_RENDERER_ACCELERATED),
        connected(false),
        game_on(false),
        connection(std::move(Socket(hostname, servname)), events, updates, connected),
        event_listener(game_on, events, multiplayer_mode, current_match),
        // renderloop(game_on, updates, state),
        // renderloop(game_on, window, renderer, updates, state),
        updater(updates, state),
        gamelobby(nullptr),
        multiplayer_mode(false)
{
    connect(&gamelobby, &lobby::create_one_player_match, this, &Client::handle_create_one_player_match);
    connect(&gamelobby, &lobby::start_match, this, &Client::handle_start_match);
}

void Client::constant_rate_loop(std::function<void(int)> processing, std::chrono::milliseconds rate)
{
    auto t1 = std::chrono::steady_clock::now();
    int it = 0;
    while (true)
    {
        processing(it);
        if (not game_on.load() or not connected.load()) break;
        auto t2 = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        auto rest = rate - elapsed;
        if (rest.count() < 0)
        {
            auto behind = -rest;
            auto lost = behind - behind % rate;
            t1 += lost;
            it += static_cast<int>(lost.count() / rate.count());
        }
        else
        {
            std::this_thread::sleep_for(rate);
        }
        t1 += rate;
        ++it;
    }
}

void Client::run() {
    try
    {
        std::chrono::milliseconds rate(16);
        connection.start_communication();

        gamelobby.show();
        app.exec();
        // if (gamelobby.was_closed_by_X()) {
        if (not game_on.load()) {
            game_on.store(false);
            connected.store(false);
            updates.close();
            events.close();
            connection.end_connection();
            return;
        }
        SDL2pp::SDL sdl(SDL_INIT_VIDEO);
        Renderer renderloop(game_on, updates, state);

        std::cout << "Client running\n";

        while (game_on.load() && connected.load())
        {
            constant_rate_loop([&](int frame)
            {
                event_listener.listen();
                renderloop.render(frame);
            }, rate);
        }
        updates.close();
        events.close();
        connection.end_connection();
        // updater.stop();
        // updater.join();
    }
    catch (ClosedQueue const& e)
    {
        std::cerr << "Se cerró la queue del cliente?! " << e.what() << '\n';
        connection.end_connection();
        updates.close();
        // updater.stop();
        // updater.join();
        events.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception thrown on a client's side: " << e.what() << '\n';
        connection.end_connection();
        updates.close();
        // updater.stop();
        // updater.join();
        events.close();
    }
    catch (...)
    {
        std::cerr << "Unknown exception on a client's side." << '\n';
        connection.end_connection();
        updates.close();
        // updater.stop();
        // updater.join();
        events.close();
    }
}

void Client::handle_create_one_player_match()
{
    QString styleSheet = 
        "QMessageBox {"
        "    background-color: #FF7900;"
        "    color: #ffffff;"
        "    max-width: 200px;"
        "    min-height: 400px;"
        "    font-size: 40px;" 
        "    font-family: Uroob;" 
        "}"
        "QMessageBox QPushButton {"
        "    background-color: #FFA500;"
        "    color: #000000;"
        "    min-width: 80px;"
        "    padding: 5px;"
        "    border-radius: 4px;"
        "    font-size: 30px;" 
        "    font-family: Uroob;"
        "}"
        "QMessageBox QPushButton:hover {"
        "    background-color: #1565c0;"
        "}";
    QMessageBox msgBox;
    msgBox.setWindowTitle("Game Mode");
    msgBox.setText("Select your game mode:");
    msgBox.addButton("Two-Player", QMessageBox::YesRole);
    msgBox.addButton("Single-Player", QMessageBox::NoRole);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIcon(QMessageBox::NoIcon);
    msgBox.setStyleSheet(styleSheet);

    int result = msgBox.exec();
    if (result == MULTIPLAYER_MODE) {
        multiplayer_mode = true;
        std::cout << "Multiplayer mode!\n";
    }
    int mode = multiplayer_mode ? 1 : 0;
    Gameaction create(1, 0, 4, 0, mode);
    events.try_push(create);
    Gamestate created_match_info = updates.pop();
    if (created_match_info.match_errors_flag == 1) {
        QMessageBox errorBox;
        errorBox.setWindowTitle("Error");
        errorBox.setText(QString::fromStdString(created_match_info.error_msg));
        errorBox.addButton(QMessageBox::Ok);
        errorBox.setStyleSheet(styleSheet);
        errorBox.exec();
        gamelobby.revert_create_button_actions();
        return;
    }
    current_id = created_match_info.player_id;
    current_match = created_match_info.match_id;
}

void Client::handle_start_match()
{
    Gameaction start(current_id, current_match, 6, 0, 0);
    events.try_push(start);
    Gamestate match_started = updates.pop();
    if (match_started.match_errors_flag == 1) {
        QMessageBox errorBox;
        errorBox.setWindowTitle("Error");
        errorBox.setText(QString::fromStdString(match_started.error_msg));
        errorBox.addButton(QMessageBox::Ok);
        errorBox.exec();
        gamelobby.revert_start_button_actions();
        return;
    }
    gamelobby.close();
    game_on.store(true);
}

void Client::handle_join_match()
{
    QString styleSheet = 
        "QMessageBox {"
        "    background-color: #FF7900;"
        "    color: #ffffff;"
        "    max-width: 200px;"
        "    min-height: 400px;"
        "    font-size: 40px;" 
        "    font-family: Uroob;" 
        "}"
        "QMessageBox QPushButton {"
        "    background-color: #FFA500;"
        "    color: #000000;"
        "    min-width: 80px;"
        "    padding: 5px;"
        "    border-radius: 4px;"
        "    font-size: 30px;" 
        "    font-family: Uroob;"
        "}"
        "QMessageBox QPushButton:hover {"
        "    background-color: #1565c0;"
        "}";
    QMessageBox msgBox;
    msgBox.setWindowTitle("Game Mode");
    msgBox.setText("Select your game mode:");
    msgBox.addButton("Two-Player", QMessageBox::YesRole);
    msgBox.addButton("Single-Player", QMessageBox::NoRole);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIcon(QMessageBox::NoIcon);
    msgBox.setStyleSheet(styleSheet);

    int result = msgBox.exec();
    if (result == MULTIPLAYER_MODE) {
        multiplayer_mode = true;
        std::cout << "Multiplayer mode!\n";
    }
    int mode = multiplayer_mode ? 1 : 0;
    Gameaction join(1, 1, 5, 0, mode);
    events.try_push(join);
}

void Client::handle_refresh_lobby()
{
    Gameaction refresh(1, 0, 7, 0, 0);
    events.try_push(refresh);
    Gamestate matches_info = updates.pop();
    
}

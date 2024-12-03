#include "client.h"

#include <iostream>
#include <SDL.h>
#include <sstream>
#include <utility>

#include "SDL2pp/SDL.hh"
#include <SDL2/SDL_mixer.h>
#include <QApplication>
#include "lobby/lobby.h"
#include <QMessageBox>
#include <QIcon>
#include <QAbstractButton>
#include <QString>
#include <QTimer>
#include "common/hands_on_sockets/liberror.h"
#include <SDL2/SDL_ttf.h>


const int MULTIPLAYER_MODE = 0;
const int RATE_60FPS = 17;
const int ID_OFFSET = 128;


Client::Client(const char* hostname, const char* servname):
        app(argc, argv),
        connected(false),
        game_on(false),
        connection(std::move(Socket(hostname, servname)), events, updates, connected, state),
        event_listener(game_on, events, multiplayer_mode, current_match),
        state(current_id, current_match_winner, multiplayer_mode),
        sdl(SDL_INIT_VIDEO),
        current_player_count(0),
        renderloop(game_on, updates, state, current_player_count),
        updater(updates, state),
        gamelobby(nullptr),
        background_music(nullptr),
        multiplayer_mode(false),
        current_id(0),
        current_match(0),
        current_match_winner(-1)
{
    connect(&gamelobby, &lobby::create_one_player_match, this, &Client::handle_create_one_player_match);
    connect(&gamelobby, &lobby::start_match, this, &Client::handle_start_match);
    connect(&gamelobby, &lobby::refresh_lobby, this, &Client::handle_refresh_lobby);
    connect(&gamelobby, &lobby::join_match, this, &Client::handle_join_match);
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
        SDL_Quit();
    }
    // Inicialización de SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " 
                  << Mix_GetError() << std::endl;
    }

    // Cargar música de fondo
    background_music = Mix_LoadMUS("resources/sonidos/musica_fondo_duck.wav");
    if (!background_music) {
        std::cerr << "Failed to load background music! SDL_mixer Error: " 
                  << Mix_GetError() << std::endl;
    } else {
        // Establecer el volumen al 50% del máximo
        Mix_VolumeMusic(MIX_MAX_VOLUME / 2);

        // Reproducir música de fondo en bucle
        if (Mix_PlayMusic(background_music, -1) == -1) {
            std::cerr << "Failed to play background music! SDL_mixer Error: " 
                      << Mix_GetError() << std::endl;
        }
    }
}

void Client::constant_rate_loop(std::function<void(int)> processing, std::chrono::milliseconds rate)
{
    auto t1 = std::chrono::steady_clock::now();
    int it = 0;
    const auto min_sleep = std::chrono::milliseconds(1);
    while (true)
    {
        if (not game_on.load() or not connected.load()) break;
        
        processing(it);
        
        auto t2 = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        auto rest = rate - elapsed;

        if (rest.count() < 0)
        {
            auto behind = -rest;
            if (behind > rate * 10)
            {
                t1 = t2;
                it = 0;
            }
            else 
            {
                auto lost = behind - (behind % rate);
                t1 += lost;
                it += static_cast<int>(lost.count() / rate.count());
            }
            std::this_thread::sleep_for(min_sleep);
            std::this_thread::yield();
        }
        else
        {
            std::this_thread::sleep_for(rest);
            t1 += rate;
        }
        ++it;
    }
}

void Client::show_winner_message()
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
    msgBox.setWindowTitle("Match ended!");
    if ((current_match_winner == current_id) and not multiplayer_mode) {
        msgBox.setText("Congratulations! You are the winner!");
    } else if ((current_match_winner == current_id) and multiplayer_mode) {
        msgBox.setText("Congratulations! You are the winner Player 1!");
    } else if (multiplayer_mode and (current_match_winner == (current_id + ID_OFFSET))) {
        msgBox.setText("Congratulations! You are the winner Player 2!");
    } else {
        msgBox.setText("The winner is player " + QString::number(current_match_winner) + "!");
    }
    msgBox.addButton(QMessageBox::Ok);
    msgBox.setStyleSheet(styleSheet);
    msgBox.exec();
}

void Client::run() {
    try
    {
        std::chrono::milliseconds rate(RATE_60FPS);
        connection.start_communication();
        while (true) {        
            gamelobby.show();
            gamelobby.reset_buttons();
            if (background_music) {
                Mix_PlayMusic(background_music, -1);
            }
            state.reset();
            app.exec();
            if (not game_on.load()) {
                Gameaction end(current_id, current_match, 8, 0, 0);
                events.try_push(end);
                game_on.store(false);
                SDL_Quit();
                TTF_Quit();
                connected.store(false);
                updates.close();
                events.close();
                connection.end_connection();
                break;
            }
            current_match_winner = -1;
            renderloop.reserve_for_players();
            renderloop.open_window();
            while (game_on.load() && connected.load())
            {
                constant_rate_loop([&](int frame)
                {
                    event_listener.listen();
                    renderloop.render(frame);
                    if (state.there_is_a_winner()) {
                        game_on.store(false);
                        show_winner_message();
                    }
                }, rate);
            }
            renderloop.close_window();
            game_on.store(false);
            Gamestate end_game;
            while (updates.try_pop(end_game)) {}
        }
    }
    catch (LibError const& e)
    {
        std::cerr << "Error de librería: " << e.what() << '\n';
    }
    catch (ClosedQueue const& e)
    {
        std::cerr << "Se cerró la queue del cliente?! " << e.what() << '\n';
        connection.end_connection();
        updates.close();
        events.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception thrown on a client's side: " << e.what() << '\n';
        connection.end_connection();
        updates.close();
        events.close();
    }
    catch (...)
    {
        std::cerr << "Unknown exception on a client's side." << '\n';
        connection.end_connection();
        updates.close();
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
    }
    int mode = multiplayer_mode ? 1 : 0;
    Gameaction create(1, 0, 4, 0, mode);
    events.try_push(create);
    Gamestate created_match_info = updates.pop();
    int i = 0;
    while ((created_match_info.type != 11) && (created_match_info.type != 10)) {
        created_match_info = updates.pop();
        i++;
    }
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
    current_player_count = match_started.player_count;
    gamelobby.close();
    game_on.store(true);
}

void Client::handle_join_match(int match_id)
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
    Gameaction join(1, match_id, 5, 0, mode);
    events.try_push(join);
    Gamestate joined_match_info = updates.pop();
    if (joined_match_info.match_errors_flag == 1) {
        QMessageBox errorBox;
        errorBox.setWindowTitle("Error");
        errorBox.setText(QString::fromStdString(joined_match_info.error_msg));
        errorBox.addButton(QMessageBox::Ok);
        errorBox.setStyleSheet(styleSheet);
        errorBox.exec();
        gamelobby.revert_join_button_actions(match_id);
        return;
    }
    current_id = joined_match_info.player_id;
    current_match = joined_match_info.match_id;
    gamelobby.set_waiting_for_match(updates, game_on, current_player_count, current_match);
    gamelobby.wait_for_match();
}

void Client::handle_refresh_lobby()
{
    Gameaction refresh(1, 0, 7, 0, 0);
    events.try_push(refresh);
    Gamestate update = updates.pop();
    gamelobby.update_lobby(update.matches_info, current_id);
}

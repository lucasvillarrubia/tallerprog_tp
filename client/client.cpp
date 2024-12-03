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
#include "common/hands_on_sockets/liberror.h"
#include <SDL2/SDL_ttf.h>


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
        connection(std::move(Socket(hostname, servname)), events, updates, connected, state),
        event_listener(game_on, events, multiplayer_mode, current_match),
        sdl(SDL_INIT_VIDEO),
        current_player_count(0),
        renderloop(game_on, updates, state, current_player_count),
        // renderloop(game_on, window, renderer, updates, state),
        updater(updates, state),
        gamelobby(nullptr),
        background_music(nullptr),
        multiplayer_mode(false)
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
    background_music = Mix_LoadMUS("resources/musica_fondo.wav");
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

// Client::~Client() {
//     // Free the music
//     if (background_music) {
//         Mix_FreeMusic(background_music);
//         background_music = nullptr;
//     }
    
//     // Close SDL_mixer
//     Mix_CloseAudio();
// }

void Client::constant_rate_loop(std::function<void(int)> processing, std::chrono::milliseconds rate)
{
    auto t1 = std::chrono::steady_clock::now();
    int it = 0;
    const auto min_sleep = std::chrono::milliseconds(1);
    while (true)
    {
        // tratando de hacer lo de martín
        // processing(it);
        // if (not game_on.load() or not connected.load()) break;
        // auto t2 = std::chrono::steady_clock::now();
        // auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        // auto rest = rate - elapsed;
        // if (rest.count() < 0)
        // {
        //     auto behind = -rest;
        //     auto lost = behind - (behind % rate);
        //     t1 += lost;
        //     it += static_cast<int>(lost.count() / rate.count());
        // }
        // else
        // {
        //     std::this_thread::sleep_for(rest);
        //     t1 += rate;
        // }

        // gpt
        // if (rest.count() > 0)
        // {
        //     std::this_thread::sleep_for(rest);
        //     t1 += rate;
        // }
        // else
        // {
        //     // std::this_thread::sleep_for(std::chrono::milliseconds(1));
        //     t1 = t2;
        // }
        // ++it;

        //claudio
        if (not game_on.load() or not connected.load()) break;
        
        processing(it);
        
        auto t2 = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        auto rest = rate - elapsed;

        if (rest.count() < 0)
        {
            // We're behind schedule
            auto behind = -rest;
            if (behind > rate * 10) // If severely behind, reset
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
            std::this_thread::sleep_for(min_sleep); // Prevent CPU spin
            std::this_thread::yield(); // Let other threads run
        }
        else
        {
            std::this_thread::sleep_for(rest);
            t1 += rate;
        }
        ++it;
    }
}

void Client::run() {
    try
    {
        std::chrono::milliseconds rate(17);
        connection.start_communication();
        while (true) {        
            gamelobby.show();
            gamelobby.reset_buttons();
            if (background_music) {
                Mix_PlayMusic(background_music, -1); // -1 means loop indefinitely
            }
            state.reset();
            app.exec();
            // if (gamelobby.was_closed_by_X()) {
            if (not game_on.load()) {
                game_on.store(false);
                SDL_Quit();
                TTF_Quit();
                connected.store(false);
                updates.close();
                events.close();
                connection.end_connection();
                // updater.stop();
                // updater.join();
                break;
            }
            // SDL2pp::SDL sdl(SDL_INIT_VIDEO);
            // Renderer renderloop(game_on, updates, state);
            renderloop.reserve_for_players();
            renderloop.open_window();
            std::cout << "Client running\n";
            // updater.start();
            while (game_on.load() && connected.load())
            {
                constant_rate_loop([&](int frame)
                {
                    event_listener.listen();
                    renderloop.render(frame);
                }, rate);
            }
            renderloop.close_window();
            // updater.stop();
            // updater.join();
            game_on.store(false);
            int i = 0;
            Gamestate end_game;
            while (updates.try_pop(end_game)) {
                i++;
            }
            std::cout << i << '\n';
        }
        // updates.close();
        // events.close();
        // connection.end_connection();
        // updater.stop();
        // updater.join();
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
    int i = 0;
    while ((created_match_info.type != 11) && (created_match_info.type != 10)) {
        created_match_info = updates.pop();
        i++;
    }
    std::cout << i << '\n';
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
    std::cout << "created match " << current_match << '\n';
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
    std::cout << "starting match " << current_match << '\n';
}

void Client::handle_join_match(int match_id)
{
    std::cout << "joining match " << match_id << '\n';
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
        gamelobby.revert_create_button_actions();
        return;
    }
    current_id = joined_match_info.player_id;
    current_match = joined_match_info.match_id;
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

void Client::handle_refresh_lobby()
{
    std::cout << "refreshing lobby\n";
    Gameaction refresh(1, 0, 7, 0, 0);
    events.try_push(refresh);
    Gamestate update = updates.pop();
    std::cout << "llegaron " << update.matches_info.size() << " partidas\n";
    gamelobby.update_lobby(update.matches_info, current_id);
}

#include "event_listener.h"

#include <SDL_events.h>
#include <QMessageBox>
#include <QString>


EventListener::EventListener(std::atomic_bool& connection, Queue<Gameaction>& q, const bool& mode, const int& match):
        connected(connection),
        events(q),
        is_multiplayer(mode),
        match_id(match) {}

void EventListener::listen() {
    try
    {
        SDL_Event event;
        if (SDL_PollEvent(&event) && connected.load())
        {
            Uint32 type = event.type;
            int type_code, key_code;
            bool second_player = false;
            if (type != SDL_QUIT) {
                SDL_Keycode key = event.key.keysym.sym;
                if (key == SDLK_ESCAPE) {
                    type = SDL_QUIT;
                }
                else if (is_multiplayer) {
                    if (not codes_by_event_type.contains(type) or (not codes_by_key_for_second_player.contains(key) and not codes_by_key.contains(key))) return;
                }
                else {
                    if (not codes_by_event_type.contains(type) || not codes_by_key.contains(key)) return;
                }
                type_code = codes_by_event_type.at(type);
                if (is_multiplayer and codes_by_key_for_second_player.contains(key)) {
                    key_code = codes_by_key_for_second_player.at(key);
                    second_player = true;
                }
                else {
                    key_code = codes_by_key.at(key);
                }
            }
            if (type == SDL_QUIT) {
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
                msgBox.setWindowTitle("Exit Match");
                msgBox.setText("Are you sure you want to exit the match?");
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                msgBox.setDefaultButton(QMessageBox::No);
                msgBox.setStyleSheet(styleSheet);
                int ret = msgBox.exec();
                if (ret == QMessageBox::Yes) {
                    Gameaction exit_action(1, match_id, 9, 9, false);
                    events.try_push(exit_action);
                    connected.store(false);
                    return;
                }
                else
                    return;
            }
            Gameaction new_action(1, match_id, type_code, key_code, second_player);
            events.try_push(new_action);
        }
    }
    catch (ClosedQueue const& e)
    {
        std::cerr << "The event listener queue was closed. " << e.what() << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught in the renderer thread: " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Unknown exception on the renderloop.\n";
    }
}

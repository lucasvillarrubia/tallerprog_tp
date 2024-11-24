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
            SDL_Keycode key = event.key.keysym.sym;
            // if (not is_multiplayer and codes_by_key_for_second_player.contains(key)) return;
            // else if ((not codes_by_event_type.contains(type) || not codes_by_key.contains(key)) and (is_multiplayer and (not codes_by_key_for_second_player.contains(key) || not codes_by_event_type.contains(type))))
            //     return;
            if (is_multiplayer) {
                if (not codes_by_event_type.contains(type) or (not codes_by_key_for_second_player.contains(key) and not codes_by_key.contains(key))) return;
            }
            else {
                if (not codes_by_event_type.contains(type) || not codes_by_key.contains(key)) return;
            }
            const int type_code = codes_by_event_type.at(event.type);
            // const int key_code = codes_by_key.at(event.key.keysym.sym);
            int key_code;
            bool second_player = false;
            if (is_multiplayer and codes_by_key_for_second_player.contains(key)) {
                std::cout << "entered second player move\n";
                key_code = codes_by_key_for_second_player.at(key);
                std::cout << "key_code: " << key_code << "\n";
                second_player = true;
            }
            else {
                key_code = codes_by_key.at(key);
            }
            Gameaction new_action(1, 1, type_code, key_code, second_player);
            events.try_push(new_action);
            if(key == SDLK_ESCAPE) {
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
                if(ret == QMessageBox::Yes)
                    connected.store(false);
            }
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

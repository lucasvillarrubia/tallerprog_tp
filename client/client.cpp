#include "client.h"

#include <iostream>
#include <sstream>
#include <utility>

#include "../common/message.h"


Client::Client(const char* hostname, const char* servname):
        connection(std::move(Socket(hostname, servname)), events, updates),
        eventloop(events),
        renderloop(updates) {}

void Client::run() {
    try
    {
        // pantalla de inicio
        // preguntar para 1 o 2 jugadores -> sólo debería activar las teclas para el jugador 2 y un
        renderloop.start();
        eventloop.start();
        // while not (quit? -> YES)
        events.close();
        updates.close();
        eventloop.stop();
        eventloop.join();
        renderloop.stop();
        renderloop.join();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception thrown on a client's side: " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Unknown exception on a client's side." << '\n';
    }
}

// ??? capaz sirve para cuando ingresan el nombre
// std::string Client::get_command_input() {
//     std::string input;
//     std::getline(std::cin, input);
//     return input;
// }

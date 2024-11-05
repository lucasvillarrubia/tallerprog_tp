#include "client.h"

#include <iostream>
#include <sstream>
#include <utility>


Client::Client(const char* hostname, const char* servname):
        connected(false),
        connection(std::move(Socket(hostname, servname)), events, updates, connected),
        eventloop(connected, events),
        // eventloop(connected, connection_ended, events),
        renderloop(connected, updates) {}

void Client::run() {
    connection.start_communication();
    // pantalla de inicio
    // preguntar para 1 o 2 jugadores -> sólo debería activar las teclas para el jugador 2 y un
    eventloop.start();
    try
    {

        renderloop.run();
        // while (connected.load()) {
        //     std::unique_lock<std::mutex> lck(mtx);
        //     connection_ended.wait(lck);
        // }
        connection.end_connection();
        events.close();
        updates.close();
        eventloop.stop();
        eventloop.join();
        // renderloop.stop();
        // renderloop.join();
    }
    catch (ClosedQueue const& e)
    {
        std::cerr << "Se cerró la queue del cliente?! " << e.what() << '\n';
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

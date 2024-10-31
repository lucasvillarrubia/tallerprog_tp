#include "client.h"

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "../common/message.h"

#include "client_exceptions.h"


#define OPENING_MESSAGE "What is your name?"
#define QUIT_GAME_COMMAND "Exit"
#define READ_COMMAND "Read"
#define PICKUP_COMMAND "Pickup"

const char END_OF_NAME = '\0';


// Client::Client(const char* hostname, const char* servname): protocol(std::move(Socket(hostname, servname))) {}
// Client::Client(const char* hostname, const char* servname): player_comms(std::move(Socket(hostname, servname)), events, updates), renderloop(updates), eventloop(events) {}
// Client::Client(const char* hostname, const char* servname): renderloop(updates), eventloop(events) {}
Client::Client(): renderloop(updates), eventloop(events) {}

void Client::run() {
    try {
        // pantalla de inicio
        // preguntar para 1 o 2 jugadores
        renderloop.start();
        eventloop.start();
    } catch (const std::exception& e) {
        std::cerr << "Exception thrown on a client's side: " << e.what() << '\n';
    } catch (...) {
        std::cerr << "Unknown exception on a client's side." << '\n';
    }
}

std::string Client::get_command_input() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

Client::~Client() {}

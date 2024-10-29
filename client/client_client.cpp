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


Client::Client(const char* hostname, const char* servname): protocol(std::move(Socket(hostname, servname))) {}

void Client::run() {
    try {
        print_ask_for_player_name();
        get_user_player_name();
        std::string command;
        while (true) {
            command = get_command_input();
            if (command == QUIT_GAME_COMMAND)
                break;
            perform_actions_by_command(command);
        }
    } catch (const ClosedSocket& e) {
    } catch (const std::exception& e) {
        std::cerr << "Exception thrown on a client's side: " << e.what() << '\n';
    } catch (...) {
        std::cerr << "Unknown exception on a client's side." << '\n';
    }
}

void Client::get_user_player_name() {
    std::string user_input;
    std::getline(std::cin, user_input);
    player_name = std::vector<char>(user_input.begin(), user_input.end());
}

std::string Client::get_command_input() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void Client::perform_actions_by_command(const std::string& input) {
    std::stringstream command(input);
    std::string action;
    std::string argument;
    command >> action;
    command >> argument;
    if (action == READ_COMMAND) {
        print_received_messages(argument);
    } else if (action == PICKUP_COMMAND) {
        send_pickup_message(argument);
    }
}

void Client::print_received_messages(const std::string& messages_qty) {
    int expected_messages = stoi(messages_qty);
    if (expected_messages < 0)
        return;
    for (int i = 0; i < expected_messages; i++) {
        Message received = protocol.receive_message();
        received.print();
    }
}

void Client::send_pickup_message(const std::string& box_type) {
    int box_id = stoi(box_type);
    if (box_id < 1 || box_id > 4)
        return;
    protocol.send_message(box_id, player_name);
}

void Client::print_ask_for_player_name() { std::cout << OPENING_MESSAGE << "\n"; }

Client::~Client() {}

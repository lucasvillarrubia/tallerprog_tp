#include "server.h"

#define END_OF_PROGRAM_CODE "q"


Server::Server(const char* servname):
        acceptor(servname, player_list, user_commands),
        match_creator(user_commands, player_list),
        // gameloop(player_list, user_commands),
        cleaner(match_list) {}

void Server::run() {
    try
    {
        acceptor.start();
        match_creator.start();
        // gameloop.start();
        // cleaner.start();
        std::string input;
        std::getline(std::cin, input);
        while (input != END_OF_PROGRAM_CODE) {
            std::getline(std::cin, input);
        }
        user_commands.close();
        // gameloop.stop();
        // gameloop.join();
        match_creator.stop();
        match_creator.join();
        acceptor.stop();
        acceptor.join();
        // cleaner.stop();
        // cleaner.join();
        player_list.clear();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught in the main thread: " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Unknown exception on the server server.\n";
    }
}

#include "server.h"

#define END_OF_PROGRAM_CODE "q"


Server::Server(const char* servname):
        acceptor(servname, player_list, user_commands),
        match_creator(user_commands, player_list, match_list),
        // gameloop(player_list, user_commands),
        cleaner(match_list) {}

void Server::run() {
    try
    {
        acceptor.start();
        match_creator.start();
        // gameloop.start();
        // cleaner.start();

        // std::string input;
        // std::getline(std::cin, input);
        // while (input != END_OF_PROGRAM_CODE) {
        //     std::getline(std::cin, input);
        // }
        std::cout << "El servidor está corriendo correctamente. Apretá 'q' para finalizarlo.\n";
        std::string input;
        while (std::getline(std::cin, input)) {
            if (input == END_OF_PROGRAM_CODE) {
                break;
            }
        }
        user_commands.close();
        // gameloop.stop();
        // gameloop.join();
        acceptor.stop();
        acceptor.join();
        match_creator.stop();
        match_creator.join();
        match_list.clear();
        // cleaner.stop();
        // cleaner.join();
        player_list.clear();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught in the main thread: " << e.what() << '\n';
        player_list.clear();
    }
    catch (...)
    {
        std::cerr << "Unknown exception on the server server.\n";
    }
}

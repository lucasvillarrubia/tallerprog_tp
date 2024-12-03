#include "server.h"

#define END_OF_PROGRAM_CODE "q"


Server::Server(const char* servname):
        acceptor(servname, player_list, user_commands),
        match_creator(user_commands, player_list, match_list),
        cleaner(match_list) {}

void Server::run() {
    try
    {
        acceptor.start();
        match_creator.start();
        std::cout << "El servidor está corriendo correctamente. Ingresá 'q' y [ENTER] para finalizarlo.\n";
        std::string input;
        while (std::getline(std::cin, input)) {
            if (input == END_OF_PROGRAM_CODE) {
                break;
            }
        }
        user_commands.close();
        acceptor.stop();
        acceptor.join();
        match_creator.stop();
        match_creator.join();
        match_list.clear();
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

#include "acceptor.h"

#include <iostream>
#include <utility>

// Para el SHUT_RDWR
#include <sys/socket.h>

#include "common/hands_on_sockets/socket.h"
#include "common/hands_on_sockets/liberror.h"


Acceptor::Acceptor(const char* servname, MonitoredList<Player*>& clients, Queue<Gameaction>& usr_cmds):
        is_running(false), acceptor(servname), player_list(clients), user_commands(usr_cmds), player_count(0) {}

void Acceptor::run() {
    try
    {
        is_running.store(true);
        while (is_running.load()) {
            Socket new_client = acceptor.accept();
            player_count++;
            std::cout << "SE CONECTÓ UN CLIENTE :D\n";
            Player* new_player = new Player(std::move(new_client), user_commands);
            new_player->set_id(player_count);
            player_list.push_back(new_player);
            new_player->start();
            player_list.clean_up();
        }
    }
    catch (const LibError& e)
    {
        is_running.store(false);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception thrown on the acceptor thread: " << e.what() << '\n';
        is_running.store(false);
    }
    catch (...)
    {
        std::cerr << "Unknown exception on the acceptor.\n";
        is_running.store(false);
    }
}

void Acceptor::stop() {
    _keep_running = false;
    is_running.store(false);
    acceptor.shutdown(SHUT_RDWR);
    acceptor.close();
}

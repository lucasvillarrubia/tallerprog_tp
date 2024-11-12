#include "local_player.h"

#include "common/hands_on_sockets/liberror.h"


LocalPlayer::LocalPlayer(Socket&& skt, Queue<Gameaction>& evn, Queue<Gamestate>& upd, std::atomic_bool& con_stat):
        is_online(con_stat),
        protocol(std::move(skt), is_online),
        sender(is_online, protocol, evn),
        receiver(is_online, protocol, upd)
{}

void LocalPlayer::start_communication()
{
    try {
        is_online.store(true);
        sender.start();
        receiver.start();
    } catch (const LibError& e) {
        std::cerr << "Liberror en el local player!\n";
        is_online.store(false);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    } catch (...) {
        std::cerr << "Unknown exception on a client's connection.\n";
    }
}

void LocalPlayer::end_connection()
{
    protocol.close_comms();
    sender.stop();
    sender.join();
    receiver.stop();
    receiver.join();
}

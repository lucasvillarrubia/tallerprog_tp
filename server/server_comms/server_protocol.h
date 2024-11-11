#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H


#include "common/comms/protocol.h"
#include "common/hands_on_sockets/socket.h"


class ServerProtocol: public Protocol
{
private:
    void send_single_duck_position_message(int, const Coordinates&);
public:
    ServerProtocol(Socket&&, std::atomic_bool&);
    void send_init_duck_message(const Gamestate&);
    void send_ducks_positions_message(const Gamestate&);
    void send_duck_state_message(const Gamestate&);
    void receive_message(Gameaction&);
    ~ServerProtocol() override = default;
};


#endif
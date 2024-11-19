#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H


#include "common/hands_on_sockets/socket.h"
#include "common/comms/protocol.h"


class ClientProtocol: public Protocol
{
private:
    void receive_init_character_message(Gamestate&);
    // void receive_single_character_position_message();
    void receive_characters_positions_message(Gamestate&);
    void receive_character_update_message(Gamestate&);
public:
    ClientProtocol(Socket&&, std::atomic_bool&);
    void send_message(const Gameaction&);
    void receive_message(Gamestate&);
    ~ClientProtocol() override = default;
};


#endif
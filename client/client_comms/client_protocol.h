#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H


#include "common/gamedata.h"
#include "common/hands_on_sockets/socket.h"
#include "common/comms/protocol.h"
#include "common/drawingdata.h"



class ClientProtocol: public Protocol
{
private:
    void receive_init_character_message(Gamestate&);
    // void receive_single_character_position_message();
    void receive_characters_positions_message(Gamestate&);
    void receive_character_update_message(Gamestate&);
    void receive_init_gun_message(Gamestate&);
    void receive_guns_positions_message(Gamestate&);
    void receive_bullet_init_message(Gamestate&);
    void receive_bullets_positions_message(Gamestate&);
    void receive_bullet_destroy_message(Gamestate&);
    void receive_match_error_message(Gamestate&);
    void receive_match_info_message(Gamestate&);
    void receive_matches_info_message(Gamestate&);
    void receive_round_ended_message(Gamestate&);
public:
    ClientProtocol(Socket&&, std::atomic_bool&);
    void send_message(const Gameaction&);
    void receive_message(Gamestate&);
    ~ClientProtocol() override = default;
};


#endif

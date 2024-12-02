#include "server_sender.h"


ServerSender::ServerSender(std::atomic_bool& con_stat, ServerProtocol& srv_prot, Queue<Gamestate>& q): Sender(con_stat), protocol(srv_prot), server_messages(q) {}

void ServerSender::send_data()
{
    Gamestate new_event = server_messages.pop();
    if (client_is_connected.load())
    {
        switch (new_event.type)
        {
        case 1:
            protocol.send_init_duck_message(new_event);
            // std::cout << "se mandó que hay un pato inicializado\n";
            break;
        case 2:
            protocol.send_ducks_positions_message(new_event);
            // std::cout << "se mandó una lista de posiciones\n";
            break;
        case 5:
        	protocol.send_init_gun_message(new_event);
        	break;
        case 6:
        	protocol.send_guns_positions_message(new_event);
        	break;
        case 7:
        	protocol.send_bullet_init_message(new_event);
        	break;
        case 8:
        	protocol.send_bullets_positions_message(new_event);
        	break;
        case 9:
        	protocol.send_bullet_destroy_message(new_event);
        	break;
        case 10:
            protocol.send_match_error_message(new_event);
            break;
        case 11:
            protocol.send_match_info_message(new_event);
            break;
        case 12:
            protocol.send_matches_info_message(new_event);
            break;
        case 15:
        	protocol.send_explosion_message(new_event);
        	std::cout << "se mandó una explosion\n";
        	break;
        case 13:
            protocol.send_round_ended_message(new_event);
            break;
        case 14:
            protocol.send_exit_message(new_event);
            break;
        default:
            protocol.send_duck_state_message(new_event);
            // std::cout << "se mandó una actualización de estado\n";
            break;
        }
    }
}



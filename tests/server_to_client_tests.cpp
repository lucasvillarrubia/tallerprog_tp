#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "common/gamedata.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <utility>

#include "protocol_test.h"


// Test de mensaje tipo 1 - Inicialización de personaje
TEST_F(ProtocolTest, send_init_character_message) {
    Gamestate message(1, 2, 3, 4, 5, 6, 7, 8, 9, {10, 11, 12});
    Gamestate received;
    server->send_init_duck_message(message);
    client->receive_message(received);
    ASSERT_EQ(received.type, message.type);
    ASSERT_EQ(received.player_id, message.player_id);
    ASSERT_FLOAT_EQ(received.pos_X, message.pos_X);
    ASSERT_FLOAT_EQ(received.pos_Y, message.pos_Y);
    ASSERT_EQ(received.is_running, message.is_running);
    ASSERT_EQ(received.is_jumping, message.is_jumping);
    ASSERT_EQ(received.is_flapping, message.is_flapping);
    ASSERT_EQ(received.move_direction, message.move_direction);
    ASSERT_EQ(received.is_alive, message.is_alive);
    ASSERT_FLOAT_EQ(received.jump_speed, message.jump_speed);
    ASSERT_EQ(received.color.r, message.color.r);
    ASSERT_EQ(received.color.g, message.color.g);
    ASSERT_EQ(received.color.b, message.color.b);
}

// Test de mensaje tipo 2 - Posiciones y velocidades de patos
TEST_F(ProtocolTest, send_ducks_positions_message) {
    std::map<int, Coordinates> positions_by_id = {{1, {1.0f, 2.0f}}, {2, {3.0f, 4.0f}}};
    std::map<int, float> speeds_by_id = {{1, 5.0f}, {2, 6.0f}};
    Gamestate message(positions_by_id, speeds_by_id);
    Gamestate received;
    server->send_ducks_positions_message(message);
    client->receive_message(received);
    ASSERT_EQ(received.type, message.type);
    for (const auto& [id, position] : message.positions_by_id) {
        ASSERT_EQ(received.positions_by_id.at(id).pos_X, position.pos_X);
        ASSERT_EQ(received.positions_by_id.at(id).pos_Y, position.pos_Y);
    }
    for (const auto& [id, speed] : message.speeds_by_id) {
        ASSERT_EQ(received.speeds_by_id.at(id), speed);
    }
}

// Test de mensaje tipo 3 - Estado de pato
TEST_F(ProtocolTest, send_duck_state_message) {
    Gamestate message(1, 1, 0, 0, 1, 0, 1, 0, 1);
    Gamestate received;
    server->send_duck_state_message(message);
    client->receive_message(received);
    ASSERT_EQ(received.type, message.type);
    ASSERT_EQ(received.player_id, message.player_id);
    ASSERT_EQ(received.is_running, message.is_running);
    ASSERT_EQ(received.is_jumping, message.is_jumping);
    ASSERT_EQ(received.is_flapping, message.is_flapping);
    ASSERT_EQ(received.move_direction, message.move_direction);
    ASSERT_EQ(received.is_alive, message.is_alive);
    ASSERT_EQ(received.is_slipping, message.is_slipping);
    ASSERT_EQ(received.is_pointing_upwards, message.is_pointing_upwards);
    ASSERT_EQ(received.is_ducking, message.is_ducking);
}

// Test de mensaje tipo 5 - Inicialización de arma
TEST_F(ProtocolTest, send_init_gun_message) {
    Gamestate message(1, 2.0f, 3.0f, 4, 5);
    Gamestate received;
    server->send_init_gun_message(message);
    client->receive_message(received);
    ASSERT_EQ(received.type, message.type);
    ASSERT_EQ(received.object_id, message.object_id);
    ASSERT_FLOAT_EQ(received.pos_X, message.pos_X);
    ASSERT_FLOAT_EQ(received.pos_Y, message.pos_Y);
    ASSERT_EQ(received.type_gun, message.type_gun);
    ASSERT_EQ(received.move_direction, message.move_direction);
}

// Test de mensaje tipo 6 - Posiciones y características de dibujp de armas
TEST_F(ProtocolTest, send_guns_positions_message) {
    std::map<int, std::pair<DrawingData, Coordinates>> guns_positions_by_id = {
        {1, {{2, 3, 4, 5}, {6.0f, 7.0f}}},
        {2, {{8, 9, 10, 11}, {12.0f, 13.0f}}}
    };
    Gamestate message(guns_positions_by_id);
    Gamestate received;
    server->send_guns_positions_message(message);
    client->receive_message(received);
    ASSERT_EQ(received.type, message.type);
    for (const auto& [id, gun_data_position] : message.guns_positions_by_id) {
        ASSERT_EQ(received.guns_positions_by_id.at(id).first.type, gun_data_position.first.type);
        ASSERT_EQ(received.guns_positions_by_id.at(id).first.right, gun_data_position.first.right);
        ASSERT_EQ(received.guns_positions_by_id.at(id).first.shooting, gun_data_position.first.shooting);
        ASSERT_EQ(received.guns_positions_by_id.at(id).first.up, gun_data_position.first.up);
        ASSERT_EQ(received.guns_positions_by_id.at(id).second.pos_X, gun_data_position.second.pos_X);
        ASSERT_EQ(received.guns_positions_by_id.at(id).second.pos_Y, gun_data_position.second.pos_Y);
    }
}

// Test de mensaje tipo 7 - Inicialización de bala
TEST_F(ProtocolTest, send_bullet_init_message) {
    Gamestate message(1, 5, 1, 0, 2.0f, 3.0f);
    Gamestate received;
    server->send_bullet_init_message(message);
    client->receive_message(received);
    ASSERT_EQ(received.type, message.type);
    ASSERT_EQ(received.object_id, message.object_id);
    ASSERT_FLOAT_EQ(received.pos_X, message.pos_X);
    ASSERT_FLOAT_EQ(received.pos_Y, message.pos_Y);
    ASSERT_EQ(received.type_gun, message.type_gun);
    ASSERT_EQ(received.move_direction, message.move_direction);
    ASSERT_EQ(received.is_pointing_upwards, message.is_pointing_upwards);
    ASSERT_FALSE(received.is_pointing_upwards);
    ASSERT_TRUE(received.move_direction);
}

// Test de mensaje tipo 8 - Posiciones de balas
TEST_F(ProtocolTest, send_bullets_positions_message) {
    std::map<int, Coordinates> bullets_positions_by_id = {{1, {1.0f, 2.0f}}, {2, {3.0f, 4.0f}}};
    Gamestate message(1, bullets_positions_by_id);
    Gamestate received;
    server->send_bullets_positions_message(message);
    client->receive_message(received);
    ASSERT_EQ(received.type, message.type);
    ASSERT_EQ(received.bullet_flag, message.bullet_flag);
    for (const auto& [id, position] : message.bullets_positions_by_id) {
        ASSERT_EQ(received.bullets_positions_by_id.at(id).pos_X, position.pos_X);
        ASSERT_EQ(received.bullets_positions_by_id.at(id).pos_Y, position.pos_Y);
    }
}

// Test de mensaje tipo 9 - Destrucción de bala
TEST_F(ProtocolTest, send_bullet_destroy_message) {
    Gamestate message(1);
    Gamestate received;
    server->send_bullet_destroy_message(message);
    client->receive_message(received);
    ASSERT_EQ(received.type, message.type);
    ASSERT_EQ(received.object_id, message.object_id);
}

// Test de mensaje tipo 10 - Mensaje de error en el partida
TEST_F(ProtocolTest, send_match_error_message) {
    Gamestate message(1, 1, "Error message");
    Gamestate received;
    server->send_match_error_message(message);
    client->receive_message(received);
    ASSERT_EQ(received.type, message.type);
    ASSERT_EQ(received.player_id, message.player_id);
    ASSERT_EQ(received.match_errors_flag, message.match_errors_flag);
    ASSERT_EQ(received.error_msg, message.error_msg);
}

// Test de mensaje tipo 11 - Mensaje de información de partida
TEST_F(ProtocolTest, send_match_info_message) {
    Gamestate message(1, 1, 3, 4);
    Gamestate received;
    server->send_match_info_message(message);
    client->receive_message(received);
    ASSERT_EQ(received.type, message.type);
    ASSERT_EQ(received.player_id, message.player_id);
    ASSERT_EQ(received.match_errors_flag, message.match_errors_flag);
    ASSERT_EQ(received.match_id, message.match_id);
    ASSERT_EQ(received.player_count, message.player_count);
}

// Test de mensaje tipo 12 - Mensaje de información de partidas
TEST_F(ProtocolTest, send_matches_info_message) {
    std::list<Gamematch> matches_info = {{1, 2, 3}, {4, 5, 6}};
    Gamestate message(1, matches_info);
    Gamestate received;
    server->send_matches_info_message(message);
    client->receive_message(received);
    ASSERT_EQ(received.type, message.type);
    ASSERT_EQ(received.player_id, message.player_id);
    for (const auto& match_info : message.matches_info) {
        ASSERT_EQ(received.matches_info.front().match_id, match_info.match_id);
        ASSERT_EQ(received.matches_info.front().players_count, match_info.players_count);
        received.matches_info.pop_front();
    }
}

// Test de mensaje tipo 13 - Mensaje de ronda finalizada
TEST_F(ProtocolTest, send_round_ended_message) {
    Gamestate message(1, 2);
    Gamestate received;
    server->send_round_ended_message(message);
    client->receive_message(received);
    ASSERT_EQ(received.type, message.type);
    ASSERT_EQ(received.player_id, message.player_id);
    ASSERT_EQ(received.round, message.round);
}

// Test de mensaje tipo 14 - Mensaje de salida
TEST_F(ProtocolTest, send_exit_message) {
    Gamestate message(1, "Exit message");
    Gamestate received;
    server->send_exit_message(message);
    client->receive_message(received);
    ASSERT_EQ(received.type, message.type);
    ASSERT_EQ(received.player_id, message.player_id);
    ASSERT_EQ(received.error_msg, message.error_msg);
}

// Test de mensaje tipo 15 - Mensaje de explosión
TEST_F(ProtocolTest, send_explosion_message) {
    Gamestate message(1, 2, 3.0f);
    Gamestate received;
    server->send_explosion_message(message);
    client->receive_message(received);
    ASSERT_EQ(received.type, message.type);
    ASSERT_EQ(received.bullet_flag, message.bullet_flag);
    ASSERT_EQ(received.object_id, message.object_id);
    ASSERT_FLOAT_EQ(received.match_errors_flag, message.match_errors_flag);
}

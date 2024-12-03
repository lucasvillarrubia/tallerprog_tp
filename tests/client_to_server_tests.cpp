#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "common/gamedata.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <utility>

#include "protocol_test.h"


// Test de mensaje tipo x - Se mantienen todos los campos desde el cliente completados
TEST_F(ProtocolTest, send_client_message) {
    Gameaction message(1, 1, 1, 1, 0);
    Gameaction received;
    client->send_message(message);
    server->receive_message(received);
    ASSERT_EQ(received.player_id, message.player_id);
    ASSERT_EQ(received.match, message.match);
    ASSERT_EQ(received.type, message.type);
    ASSERT_EQ(received.key, message.key);
    ASSERT_EQ(received.is_multiplayer, message.is_multiplayer);
}

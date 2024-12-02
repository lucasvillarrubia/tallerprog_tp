#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "client/client_comms/client_protocol.h"
#include "server/server_comms/server_protocol.h"
#include "common/hands_on_sockets/socket.h"
#include "common/hands_on_sockets/liberror.h"
// #include <gmock-spec-builders.h>
#include "socket_interface.h"
#include "mock_socket.h"
#include "test_acceptor_thread.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <utility>


int getAvailablePort() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = 0; // Let the OS choose the port

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sockfd);
        return -1;
    }

    socklen_t addrlen = sizeof(addr);
    if (getsockname(sockfd, (struct sockaddr*)&addr, &addrlen) < 0) {
        perror("getsockname");
        close(sockfd);
        return -1;
    }

    int port = ntohs(addr.sin_port);
    close(sockfd);
    return port;
}

// class MockSocket : public ISocket {
// public:
//     // MockSocket() : Socket(0) {}
//     MockSocket(const char* port) : ISocket(port) {}
//     MockSocket(const char* port, int flag) : ISocket(port, flag) {}
//     // MockSocket(const char* servname) : ISocket(servname) {}
//     MOCK_METHOD(int, sendsome, (const void* data, unsigned int sz, bool* was_closed), (override));
//     MOCK_METHOD(int, recvsome, (void* data, unsigned int sz, bool* was_closed), (override));
//     MOCK_METHOD(int, sendall, (const void* data, unsigned int sz, bool* was_closed), (override));
//     MOCK_METHOD(int, recvall, (void* data, unsigned int sz, bool* was_closed), (override));
//     MOCK_METHOD(void, shutdown, (int how), (override));
//     MOCK_METHOD(int, close, (), (override));
// };


class ProtocolTest : public ::testing::Test {
protected:
    std::atomic_bool connection_status{true};
    std::unique_ptr<Socket> server_mock_socket;
    std::unique_ptr<Socket> acceptor_mock_socket;
    std::unique_ptr<Socket> client_mock_socket;
    ServerProtocol* server;
    ClientProtocol* client;
    int port;
    std::mutex mtx;
    std::condition_variable cv;
    bool server_ready = false;
    bool client_ready = false;

    void SetUp() override {
        port = getAvailablePort();
        ASSERT_NE(port, -1) << "Failed to get an available port for the protocol";
        const std::string port_str = std::to_string(port);
        acceptor_mock_socket = std::make_unique<Socket>(port_str.c_str());

        // Start server thread
        std::thread server_thread([this]() {
            {
                std::unique_lock<std::mutex> lock(mtx);
                server_ready = true;
                cv.notify_all();
            }
            server_mock_socket = std::make_unique<Socket>(std::move(acceptor_mock_socket->accept()));
            server = new ServerProtocol(std::move(*server_mock_socket), connection_status);
        });

        // Wait for server to be ready
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]() { return server_ready; });
        }

        // Create client socket in a separate thread
        std::thread client_thread([this, port_str]() {
            while (not client_ready) {
                try {
                    client_mock_socket = std::make_unique<Socket>("localhost", port_str.c_str());
                    client = new ClientProtocol(std::move(*client_mock_socket), connection_status);
                    client_ready = true;
                } catch (const LibError& e) {
                    continue;
                }
            }
        });

        // Ensure the client thread completes
        client_thread.join();
        // Ensure the server thread completes
        server_thread.join();
    }

    void TearDown() override {
        delete server;
        delete client;
    }
};



class ClientProtocolTest : public ::testing::Test {
protected:
    std::atomic_bool connection_status{true};
    std::unique_ptr<MockSocket> mock_socket;
    ClientProtocol* client_protocol;
    int port;

    void SetUp() override {
        port = getAvailablePort();
        ASSERT_NE(port, -1) << "Failed to get an available port for the client";
        const std::string port_str = std::to_string(port);
        mock_socket = std::make_unique<MockSocket>(port_str.c_str());
        client_protocol = new ClientProtocol(std::move(*mock_socket), connection_status);
    }

    void TearDown() override {
        delete client_protocol;
    }
};

class ServerProtocolTest : public ::testing::Test {
protected:
    std::atomic_bool connection_status{true};
    std::unique_ptr<MockSocket> mock_socket;
    ServerProtocol* server_protocol;
    int port;

    void SetUp() override {
        port = getAvailablePort();
        ASSERT_NE(port, -1) << "Failed to get an available port for the server";
        const std::string port_str = std::to_string(port);
        mock_socket = std::make_unique<MockSocket>(port_str.c_str());
        server_protocol = new ServerProtocol(std::move(*mock_socket), connection_status);
    }

    void TearDown() override {
        delete server_protocol;
    }
};

// TEST_F(ClientProtocolTest, SendMessage) {
//     Gameaction message{1, 2, 3, 4, true};

//     EXPECT_CALL(*mock_socket, sendall(testing::_, sizeof(uint8_t), testing::_)).Times(5);

//     client_protocol->send_message(message);
// }

// TEST_F(ServerProtocolTest, ReceiveMessage) {
//     Gameaction received;
//     EXPECT_CALL(*mock_socket, recvall(testing::_, sizeof(uint8_t), testing::_)).Times(5);

//     server_protocol->receive_message(received);
// }

TEST_F(ProtocolTest, SendInitCharacterMessage) {
    Gamestate message{1, 2, 3, 4, 5, 6, 7, 8, 9, {10, 11, 12}};
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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    // ::testing::FLAGS_gtest_catch_exceptions = false;
    // ::testing::FLAGS_gtest_output = "xml:";
    return RUN_ALL_TESTS();
}
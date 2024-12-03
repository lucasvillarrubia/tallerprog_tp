#ifndef SERVER_TO_CLIENT_TESTS_H
#define SERVER_TO_CLIENT_TESTS_H


#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "client/client_comms/client_protocol.h"
#include "server/server_comms/server_protocol.h"
#include "common/hands_on_sockets/socket.h"
#include "common/hands_on_sockets/liberror.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <utility>


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
        std::thread server_thread([this]() {
            {
                std::unique_lock<std::mutex> lock(mtx);
                server_ready = true;
                cv.notify_all();
            }
            server_mock_socket = std::make_unique<Socket>(std::move(acceptor_mock_socket->accept()));
            server = new ServerProtocol(std::move(*server_mock_socket), connection_status);
        });
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]() { return server_ready; });
        }
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
        client_thread.join();
        server_thread.join();
    }

    void TearDown() override {
        delete server;
        delete client;
    }

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
};


#endif
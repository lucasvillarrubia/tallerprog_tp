#ifndef TEST_ACCEPTOR_THREAD_H
#define TEST_ACCEPTOR_THREAD_H


#include "common/hands_on_threads/thread.h"
#include "mock_socket.h"


class TestAcceptorThread : public Thread {
private:
    std::unique_ptr<MockSocket>& client_mock_socket;
    const int& port;
public:
    explicit TestAcceptorThread(std::unique_ptr<MockSocket>& skt, const int& port) : client_mock_socket(skt), port(port) {}
    void run() override {
        client_mock_socket = std::make_unique<MockSocket>(std::to_string(port).c_str());
        stop();
    }
};


#endif
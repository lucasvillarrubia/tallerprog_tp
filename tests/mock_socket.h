#ifndef MOCK_SOCKET_H
#define MOCK_SOCKET_H


#include "common/hands_on_sockets/socket.h"
#include "gmock/gmock.h"
#include "socket_interface.h"

#include <utility>


class MockSocket : public ISocket {
public:
    // MockSocket() : Socket(0) {}
    MockSocket(const char* port) : ISocket(port) {}
    MockSocket(const char* port, int flag) : ISocket(port, flag) {}
    //acceptor constructor
    MockSocket(Socket&& accepted) : ISocket(std::move(accepted)) {}
    // MockSocket(const char* servname) : ISocket(servname) {}
    MOCK_METHOD(int, sendsome, (const void* data, unsigned int sz, bool* was_closed), (override));
    MOCK_METHOD(int, recvsome, (void* data, unsigned int sz, bool* was_closed), (override));
    MOCK_METHOD(int, sendall, (const void* data, unsigned int sz, bool* was_closed), (override));
    MOCK_METHOD(int, recvall, (void* data, unsigned int sz, bool* was_closed), (override));
    MOCK_METHOD(void, shutdown, (int how), (override));
    MOCK_METHOD(int, close, (), (override));
};


#endif
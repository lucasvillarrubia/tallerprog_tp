#ifndef ISOCKET_H
#define ISOCKET_H


#include <utility>


// class ISocket {
// public:
//     virtual ~ISocket() = default;
//     virtual int sendsome(const void* data, unsigned int sz, bool* was_closed) = 0;
//     virtual int recvsome(void* data, unsigned int sz, bool* was_closed) = 0;
//     virtual int sendall(const void* data, unsigned int sz, bool* was_closed) = 0;
//     virtual int recvall(void* data, unsigned int sz, bool* was_closed) = 0;
//     virtual void shutdown(int how) = 0;
//     virtual int close() = 0;
// };

#include "common/hands_on_sockets/socket.h"

class ISocket : public Socket {
public:
    // ISocket() : Socket("localhost", nullptr) {}
    // ISocket() : Socket("0") {}
    ISocket(const char* servname) : Socket(servname) {}
    ISocket(const char* port, int) : Socket("localhost", port) {}
    ISocket(Socket&& accepted) : Socket(std::move(accepted)) {}
    // ISocket(const char* servname) : Socket("localhost", servname) {}
    virtual ~ISocket() = default;
    virtual int sendsome(const void* data, unsigned int sz, bool* was_closed) = 0;
    virtual int recvsome(void* data, unsigned int sz, bool* was_closed) = 0;
    virtual int sendall(const void* data, unsigned int sz, bool* was_closed) = 0;
    virtual int recvall(void* data, unsigned int sz, bool* was_closed) = 0;
    virtual void shutdown(int how) = 0;
    virtual int close() = 0;
};

#endif
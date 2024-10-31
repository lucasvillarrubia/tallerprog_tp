#ifndef SOCKET_H
#define SOCKET_H

class Socket {
private:
    int skt;
    bool closed;
    explicit Socket(int skt);
    void chk_skt_or_fail() const;


public:
    Socket(const char* hostname, const char* servname);

    explicit Socket(const char* servname);

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&&);
    Socket& operator=(Socket&&);

    int sendsome(const void* data, unsigned int sz, bool* was_closed);

    int recvsome(void* data, unsigned int sz, bool* was_closed);

    int sendall(const void* data, unsigned int sz, bool* was_closed);

    int recvall(void* data, unsigned int sz, bool* was_closed);

    Socket accept();

    void shutdown(int how);

    int close();

    ~Socket();
};
#endif
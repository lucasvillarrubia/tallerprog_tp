#ifndef RESOLVER_H
#define RESOLVER_H

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

class Resolver {
private:
    struct addrinfo* result;
    struct addrinfo* _next;

    void chk_addr_or_fail() const;

public:
    Resolver(const char* hostname, const char* servname, bool is_passive);

    Resolver(const Resolver&) = delete;
    Resolver& operator=(const Resolver&) = delete;

    Resolver(Resolver&&);
    Resolver& operator=(Resolver&&);

    bool has_next();

    struct addrinfo* next();

    ~Resolver();
};
#endif

#ifndef RESOLVER_ERROR_H
#define RESOLVER_ERROR_H

#include <exception>


class ResolverError: public std::exception {
private:
    int gai_errno;

public:
    explicit ResolverError(int gai_errno);

    virtual const char* what() const noexcept;

    virtual ~ResolverError();
};
#endif

#ifndef LIB_ERROR_H
#define LIB_ERROR_H

#include <exception>

class LibError: public std::exception {
    char msg_error[256];

public:
    LibError(int error_code, const char* fmt, ...) noexcept;

    virtual const char* what() const noexcept;

    virtual ~LibError();
};

#endif

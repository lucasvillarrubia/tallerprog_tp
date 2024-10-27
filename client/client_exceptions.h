#ifndef CLIENT_EXCEPTIONS_H
#define CLIENT_EXCEPTIONS_H


#include <exception>
#include <stdexcept>


// Excepción lanzada sólo del lado del cliente cuando se cierra su comunicación externamente.

struct ClosedSocket: public std::runtime_error {
    ClosedSocket(): std::runtime_error("The socket is closed.") {}
};


#endif
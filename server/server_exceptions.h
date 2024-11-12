//
// Created by lucas on 09/11/24.
//

#ifndef SERVER_EXCEPTIONS_H
#define SERVER_EXCEPTIONS_H
#include <stdexcept>

struct IdNotChangeable: std::runtime_error {
    IdNotChangeable(): std::runtime_error("Id already set and cannot be changed.") {}
};

#endif //SERVER_EXCEPTIONS_H

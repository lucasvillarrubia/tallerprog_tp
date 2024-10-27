#ifndef COMMAND_H
#define COMMAND_H


#include <vector>


// En el tp de Sockets tuve la corrección de que era redundante una clase para un DTO.
// El struct Command solo tiene nombre de jugador y un id de comando.


struct Command {
    std::vector<char> player;
    int id;
    Command(const std::vector<char>& name, int _id): player(name), id(_id) {}
    Command(): id(0) {}
};


#endif
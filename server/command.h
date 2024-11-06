#ifndef COMMAND_H
#define COMMAND_H


#include <vector>


struct Command {
    std::vector<char> player;
    int id;
    Command(const std::vector<char>& name, int _id): player(name), id(_id) {}
    Command(): id(0) {}
};


#endif
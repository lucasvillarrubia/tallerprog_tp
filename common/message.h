#ifndef MESSAGE_H
#define MESSAGE_H


#include <map>
#include <string>
#include <vector>


// Clase "Printer" que es casi un DTO pero crea el mismo mensaje para el cliente y el servidor
//      y puede imprimirse por salida estándar.
// RAII: no permite copia.


class Message {
private:
    std::string content;
    std::map<int, std::string> rewards_by_id = {
            {0x10, "Bazooka"}, {0x11, "Chainsaw"}, {0x12, "Death ray"}, {0x13, "Shotgun"}};

public:
    Message();
    Message(std::vector<char>&, int);
    void print();
    Message(const Message&) = delete;
    Message& operator=(const Message&) = delete;
    ~Message();
};


#endif
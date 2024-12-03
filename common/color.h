#ifndef COLOR_H
#define COLOR_H


// #include <SDL_stdinc.h>
#include <cstdint>


struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    Color() : r(255), g(255), b(255) {}
    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
};


#endif // COLOR_H
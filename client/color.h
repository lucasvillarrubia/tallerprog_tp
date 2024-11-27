#ifndef COLOR_H
#define COLOR_H


#include <SDL_stdinc.h>


struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Color() : r(0), g(0), b(0) {}
    Color(Uint8 r, Uint8 g, Uint8 b) : r(r), g(g), b(b) {}
};


#endif // COLOR_H
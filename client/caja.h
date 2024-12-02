#ifndef CAJA_H
#define CAJA_H

class Caja {
public:
    float x, y;

    bool is_exploding;
    // Constructor para inicializar x e y
    Caja(float posX, float posY) : x(posX), y(posY), is_exploding(false) {}
};

#endif //CAJA_H
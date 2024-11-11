#ifndef RECTANGULO_H
#define RECTANGULO_H
#include <cstdint>


class Rectangulo {
public:
    float x, y, ancho, alto;

    Rectangulo() : x(0.0f), y(0.0f), ancho(0.0f), alto(0.0f) {}

    Rectangulo(float x, float y, float ancho, float alto)
        : x(x), y(y), ancho(ancho), alto(alto) {}

    // Detección de colisión general (ambos ejes)
    bool hay_colision(const Rectangulo& otro) const {
        return (this->x < otro.x + otro.ancho && this->x + this->ancho > otro.x &&
                this->y < otro.y + otro.alto && this->y + this->alto > otro.y);
    }

    // Detección de colisión en el eje X (horizontal)
    bool hay_colision_x(const Rectangulo& otro) const {
        return (this->x < otro.x + otro.ancho && this->x + this->ancho > otro.x);
    }

    // Detección de colisión en el eje Y (vertical)
    bool hay_colision_y(const Rectangulo& otro) const {
        return (this->y < otro.y + otro.alto && this->y + this->alto > otro.y);
    }
};



#endif

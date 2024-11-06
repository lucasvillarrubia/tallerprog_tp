#ifndef MAPA_H
#define MAPA_H

#include "SDL2pp/SDL2pp.hh"
#include <map>
#include <string>

class Mapa {
private:
    SDL2pp::Texture background;  // Textura de fondo del mapa
    int ancho_ventana;
    int alto_ventana;

public:
    // Constructor que recibe el renderer y la ruta de la textura del fondo
    Mapa(SDL2pp::Renderer &render, const std::string &fondo_path)
            : background(render, fondo_path),
              ancho_ventana(render.GetOutputWidth()),
              alto_ventana(render.GetOutputHeight()) {}

    // Dibuja el fondo en la ventana
    void dibujar_fondo(SDL2pp::Renderer &render) {
        render.Copy(background, SDL2pp::Rect(0, 0, ancho_ventana, alto_ventana));
    }

    ~Mapa() {}
};

#endif
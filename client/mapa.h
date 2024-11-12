#ifndef MAPA_H
#define MAPA_H

#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "common/rectangulo.h"  // Asegúrate de incluir tu archivo de Rectangulo
#include <SDL2pp/SDL2pp.hh>

class Mapa {
private:
    // Un mapa de entidades donde cada clave es un nombre (por ejemplo "piso") y el valor es un Rectángulo
    std::map<std::string, Rectangulo> entidades;

    // Un mapa para asociar cada entidad con una textura SDL
    std::map<std::string, SDL_Texture*> texturas;

public:
    // Constructor
    Mapa() {

    }
    // Destructor para liberar texturas
    ~Mapa() {
      
    }

    void dibujar_fondo(SDL2pp::Renderer& renderer, const std::string& path) {
        SDL2pp::Texture background(renderer, path);
        renderer.Copy(background, SDL2pp::Rect(0, 0, renderer.GetOutputWidth(), renderer.GetOutputHeight()));
    }
   

    // Método para agregar una entidad al mapa con su nombre y su Rectángulo
    void agregarEntidad(const std::string& nombre, const Rectangulo& rectangulo) {
        entidades[nombre] = rectangulo;
    }

    // Método para asignar una textura a una entidad por nombre
    void asignarTextura(const std::string& nombre, SDL_Texture* textura) {
        texturas[nombre] = textura;
    }

    // Método para renderizar las entidades en el mapa
    void renderizar(SDL_Renderer* renderer) {
    for (const auto& entidad : entidades) {
        const std::string& nombre = entidad.first;
        const Rectangulo& rect = entidad.second;

        // Obtener el tipo base del nombre (p.ej., "piso" de "piso_1")
        std::string tipo = nombre.substr(0, nombre.find("_"));

        // Buscar la textura correspondiente al tipo
        if (texturas.find(tipo) != texturas.end()) {
            SDL_Texture* textura = texturas[tipo];
            SDL_Rect dstRect = {static_cast<int>(rect.x), static_cast<int>(rect.y), static_cast<int>(rect.ancho), static_cast<int>(rect.alto)};
            SDL_RenderCopy(renderer, textura, nullptr, &dstRect);
        }
    }
}
};

#endif // MAPA_H


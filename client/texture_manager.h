#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "SDL2pp/Renderer.hh"
#include "SDL2pp/SDL.hh"
#include "SDL2pp/Surface.hh"
#include "SDL2pp/Texture.hh"
#include "SDL2pp/Window.hh"
#include <map>
#include <memory>
#include <string>

class TextureManager {
private:
    SDL2pp::Renderer& renderer;
    std::map<std::string, std::unique_ptr<SDL2pp::Texture>> texturas;

public:
    TextureManager(SDL2pp::Renderer& renderer) : renderer(renderer) {
        cargarTexturas();
    }

    void cargarTexturas() {
        // Cargamos todas las texturas necesarias
        cargarTextura("piso", "resources/piso.png");
        cargarTextura("pared", "resources/pared.png");
    }

    void cargarTextura(const std::string& nombre, const std::string& path) {
        SDL2pp::Surface surface(path);
        texturas[nombre] = std::make_unique<SDL2pp::Texture>(renderer, surface);
    }

    SDL2pp::Texture* getTextura(const std::string& nombre) {
        auto it = texturas.find(nombre);
        if (it != texturas.end()) {
            return it->second.get();
        }
        return nullptr;
    }
};

#endif // TEXTUREMANAGER_H
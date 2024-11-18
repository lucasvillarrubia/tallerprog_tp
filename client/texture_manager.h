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
    std::unordered_map<int, std::unique_ptr<SDL2pp::Texture>> duck_sprites;
    std::unordered_map<int, std::unique_ptr<SDL2pp::Texture>> duck_sprites_volando;
    
public:
    TextureManager(SDL2pp::Renderer& renderer) : renderer(renderer) {
        cargarTexturas();
        cargarDuckSprites();
        cargarDuckSpritesVolando();
    }

    void cargarTexturas() {
        cargarTextura("piso_laguna", "resources/piso_laguna.png");
        cargarTextura("pared_laguna", "resources/pared_laguna.png");
        cargarTextura("piso_azul", "resources/piso_azul.png");
        cargarTextura("pared_azul", "resources/pared_azul.png");
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

    void cargarDuckSprites() {
        cargarDuckSprite(1, "resources/White_Duck_Sprites.png");
        cargarDuckSprite(2, "resources/Blue_Duck_Sprites.png");
        cargarDuckSprite(3, "resources/Pink_Duck_Sprites.png");
        cargarDuckSprite(4, "resources/Yellow_Duck_Sprites.png");
    }
    
    void cargarDuckSpriteVolando(int id, const std::string& path) {
        SDL2pp::Surface surface(path);
        duck_sprites_volando[id] = std::make_unique<SDL2pp::Texture>(renderer, surface);
    }

    void cargarDuckSpritesVolando() {
        cargarDuckSpriteVolando(1, "resources/aleteo.png");
    }

    void cargarDuckSprite(int id, const std::string& path) {
        SDL2pp::Surface surface(path);
        duck_sprites[id] = std::make_unique<SDL2pp::Texture>(renderer, surface);
    }

    SDL2pp::Texture* getDuckSprite(int id) {
        auto it = duck_sprites.find(id);
        if (it != duck_sprites.end()) {
            return it->second.get();
        }
        return nullptr; 
    }

    SDL2pp::Texture* getDuckSpriteVolando(int id) {
    auto it = duck_sprites_volando.find(id);
    if (it != duck_sprites_volando.end()) {
        return it->second.get();
    }
    return nullptr;
    }
};

#endif // TEXTUREMANAGER_H
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
#include <SDL2/SDL_mixer.h>

#include "common/color.h"


class TextureManager {
private:
    SDL2pp::Renderer& renderer;
    std::map<std::string, std::unique_ptr<SDL2pp::Texture>> texturas;
    std::unordered_map<int, std::unique_ptr<SDL2pp::Texture>> duck_sprites;
    std::unordered_map<int, std::unique_ptr<SDL2pp::Texture>> duck_sprites_volando;
    std::unordered_map<int, std::unique_ptr<SDL2pp::Texture>> duck_sprites_ducking;
    std::map<std::string, std::unique_ptr<SDL2pp::Texture>> guns_sprites;
    std::map<std::string, std::unique_ptr<SDL2pp::Texture>> items_sprites;
    std::map<std::string, Mix_Chunk*> preloaded_sounds;
    std::map<int, Color> duck_colors_by_id;
public:
    TextureManager(SDL2pp::Renderer& renderer) : renderer(renderer) {
        if (Mix_Init(MIX_INIT_MP3) < 0) {
            std::cerr << "SDL_mixer initialization error: " << Mix_GetError() << std::endl;
        }
        
        // Open audio device
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "SDL_mixer audio device error: " << Mix_GetError() << std::endl;
        }
        cargarTexturas();
        cargarDuckSprites();
        cargarDuckSpritesVolando();
        cargarDuckSpritesDucking();
        cargarGunsSprites();
        cargarItemsSprites();
        cargarSonidos();
    }

    ~TextureManager() {
        // Free preloaded sounds
        for (auto& sound : preloaded_sounds) {
            if (sound.second) {
                Mix_FreeChunk(sound.second);
            }
        }
        
        // Close audio device and quit SDL_mixer
        Mix_CloseAudio();
        Mix_Quit();
    }

    void cargarSonidos() {
        preloadSound("salto", "resources/sonidos/salto.wav");
    }

    void cargarTexturas() {
        cargarTextura("piso_laguna", "resources/piso_laguna.png");
        cargarTextura("pared_laguna", "resources/pared_laguna.png");
        cargarTextura("piso_azul", "resources/piso_azul.png");
        cargarTextura("pared_azul", "resources/pared_azul.png");
        cargarTextura("piso_minecraft", "resources/piso_minecraft.png");
        cargarTextura("piso_desierto", "resources/piso_desierto.png");
        cargarTextura("piso_ciudad", "resources/piso_ciudad.png");

        // Fondos
        cargarTextura("mapa_azul", "resources/fondo_azul.png");
        cargarTextura("mapa_laguna", "resources/fondo_laguna.png");
        cargarTextura("mapa_minecraft", "resources/fondo_minecraft.png");
        cargarTextura("mapa_ciudad", "resources/fondo_ciudad.png");
        cargarTextura("mapa_desierto", "resources/fondo_desierto.png");
    }

    void preloadSound(const std::string& soundName, const std::string& soundPath) {
        // Check if sound is already preloaded
        if (preloaded_sounds.find(soundName) != preloaded_sounds.end()) {
            return;
        }

        Mix_Chunk* sound = Mix_LoadWAV(soundPath.c_str());
        if (!sound) {
            std::cerr << "Error preloading sound file " << soundPath << ": " << Mix_GetError() << std::endl;
            return;
        }

        preloaded_sounds[soundName] = sound;
    }

    void playPreloadedSound(const std::string& soundName, int volume = MIX_MAX_VOLUME) {
        auto it = preloaded_sounds.find(soundName);
        if (it == preloaded_sounds.end()) {
            std::cerr << "Sound not preloaded: " << soundName << std::endl;
            return;
        }

        Mix_VolumeChunk(it->second, volume);
        int channel = Mix_PlayChannel(-1, it->second, 0);
        if (channel == -1) {
            std::cerr << "Error playing sound: " << Mix_GetError() << std::endl;
        }
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

    void cargarItemSprite(const std::string& nombre, const std::string& path) {
        SDL2pp::Surface surface(path);
        items_sprites[nombre] = std::make_unique<SDL2pp::Texture>(renderer, surface);
    }

    void cargarItemsSprites() {
        cargarItemSprite("armadura", "resources/armadura.png");
        cargarItemSprite("casco", "resources/casco.png");
        cargarItemSprite("caja", "resources/caja.png");
    }

    void cargarDuckSprites() {
        cargarDuckSprite(0, {}, "resources/White_Duck_Sprites.png");
    }

    SDL2pp::Texture* getItem(const std::string& nombre) {
        auto it = items_sprites.find(nombre);
        if (it != items_sprites.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    void cargarDuckSpritesDucking() {
        cargarDuckSpriteDucking(0, "resources/ducking.png");
    }

    void cargarDuckSpriteDucking(int id, const std::string& path) {
        SDL2pp::Surface surface(path);
        if (id != 0) {
            Color color = duck_colors_by_id.at(id);
            changeNearWhitePixelsToColor(surface, color.r, color.g, color.b);
        }
        duck_sprites_ducking[id] = std::make_unique<SDL2pp::Texture>(renderer, surface);
    }
    
    void cargarDuckSpriteVolando(int id, const std::string& path) {
        SDL2pp::Surface surface(path);
        if (id != 0) {
            Color color = duck_colors_by_id.at(id);
            changeNearWhitePixelsToColor(surface, color.r, color.g, color.b);
        }
        duck_sprites_volando[id] = std::make_unique<SDL2pp::Texture>(renderer, surface);
    }

    void cargarDuckSpritesVolando() {
        cargarDuckSpriteVolando(0, "resources/aleteo.png");
    }

    void changeNearWhitePixelsToColor(SDL2pp::Surface& surface, Uint8 r, Uint8 g, Uint8 b, Uint8 tolerance = 60) {
        SDL_LockSurface(surface.Get());
        Uint32* pixels = static_cast<Uint32*>(surface.Get()->pixels);
        int width = surface.Get()->w;
        int height = surface.Get()->h;
        int pitch = surface.Get()->pitch / 4;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Uint32* pixel = pixels + y * pitch + x;
                Uint8 red, green, blue, alpha;
                SDL_GetRGBA(*pixel, surface.Get()->format, &red, &green, &blue, &alpha);
                if (red >= 255 - tolerance && green >= 255 - tolerance && blue >= 255 - tolerance) {
                    *pixel = SDL_MapRGBA(surface.Get()->format, r, g, b, alpha);
                }
            }
        }
        SDL_UnlockSurface(surface.Get());
    }

    void cargarDuckSprite(int id, const Color color, const std::string& path) {
        SDL2pp::Surface surface(path);
        // static bool seedInitialized = false;
        // if (!seedInitialized) {
        //     std::srand(static_cast<unsigned int>(std::time(nullptr)));
        //     seedInitialized = true;
        // }
        if (id != 0) {
            // duck_sprites.at(id).get()->SetColorMod(color.r, color.g, color.b);
            changeNearWhitePixelsToColor(surface, color.r, color.g, color.b);
            // duck_sprites.at(id).get()->SetBlendMode(SDL_BLENDMODE_ADD);
            // Uint8 brightness = static_cast<Uint8>(255 * (1.2f - 1));
            // duck_sprites.at(id).get()->SetColorMod(255, 255, 255);
            // duck_sprites.at(id).get()->SetAlphaMod(brightness);
            duck_colors_by_id[id] = color;
        }
        duck_sprites[id] = std::make_unique<SDL2pp::Texture>(renderer, surface);
    }

    SDL2pp::Texture* getDuckSprite(int id, const Color color) {
        auto it = duck_sprites.find(id);
        if (it == duck_sprites.end()) {
            cargarDuckSprite(id, color, "resources/White_Duck_Sprites.png");
            return duck_sprites.at(id).get();
        }
        else {
            return it->second.get();
        }
        return nullptr; 
    }

    SDL2pp::Texture* getDuckSpriteDucking(int id) {
    auto it = duck_sprites_ducking.find(id);
    if (it == duck_sprites_ducking.end()) {
        cargarDuckSpriteDucking(id, "resources/ducking.png");
        return duck_sprites_ducking.at(id).get();
    }
    else {
        return it->second.get();
    }
    return nullptr;
    }

    SDL2pp::Texture* getDuckSpriteVolando(int id) {
        auto it = duck_sprites_volando.find(id);
        if (it == duck_sprites_volando.end()) {
            cargarDuckSpriteVolando(id, "resources/aleteo.png");
            return duck_sprites_volando.at(id).get();
        }
        else {
            return it->second.get();
        }
        return nullptr;
    }
    
    void cargarGunsSprites() {
    	cargarGunSprite("arrojadizos", "resources/PC Computer - Duck Game - Grenades.png");
    	cargarGunSprite("lasers", "resources/PC Computer - Duck Game - Laser.png");
    	cargarGunSprite("ak47", "resources/ak47.png");
    	cargarGunSprite("pistolas", "resources/PC Computer - Duck Game - Pistol.png");
    	cargarGunSprite("sniper", "resources/sniper.png");
    }
    
    void cargarGunSprite(const std::string& nombre, const std::string& path) {
    	SDL2pp::Surface surface(path);
        guns_sprites[nombre] = std::make_unique<SDL2pp::Texture>(renderer, surface);
    }
    
    SDL2pp::Texture* getGunSprite(const std::string& nombre) {
    	auto it = guns_sprites.find(nombre);
        if (it != guns_sprites.end()) {
            return it->second.get();
        }
        return nullptr;
    }
};

#endif // TEXTUREMANAGER_H

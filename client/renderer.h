#ifndef RENDERER_H
#define RENDERER_H


#include "client/character.h"
#include "common/gamedata.h"
#include "common/hands_on_threads/queue.h"
#include "common/hands_on_threads/thread.h"
#include "SDL2pp/Renderer.hh"
#include "SDL2pp/Window.hh"
#include "state_manager.h"


class Renderer
{
private:
    std::atomic_bool& connected;
    SDL2pp::Window& window;
    SDL2pp::Renderer& renderer;
    Queue<Gamestate>& updates_feed;
    StateManager& state;
    float zoom_factor;

    

public:
    Renderer(std::atomic_bool&, SDL2pp::Window&, SDL2pp::Renderer&, Queue<Gamestate>&, StateManager&);
    
    float get_zoom_factor() const { return zoom_factor; }
    void set_zoom_factor(float new_zoom_factor) { zoom_factor = new_zoom_factor; }

    void draw_character(SDL2pp::Texture&, Character&, int, const float zoom_offset_x, const float zoom_offset_y);
    void run(int);
    void calculate_zoom_offsets(float& offset_x, float& offset_y, float avg_x, float avg_y);
    void dibujar_mapa(const float zoom_offset_x, const float zoom_offset_y);
    void calculate_required_zoom(const std::vector<Coordinates>& duck_positions);
    ~Renderer() = default;
};


#endif

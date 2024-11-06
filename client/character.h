#ifndef CHARACTER_H
#define CHARACTER_H


#include "coordinates.h"
#include "item_box.h"

// hola



class Character {
private:
    float pos_X;
    float pos_Y;
    bool is_running;
    bool is_jumping;
    bool is_flapping;
    bool moving_right;
    float jump_velocity;
    int movement_phase;
    bool is_ducking;
    bool is_grabbing;

public:
    Character();
    void update_position(unsigned int, unsigned int, int);
    bool is_moving_to_the_right() const;
    Coordinates get_coordinates() const;
    int get_movement_phase () const;
    bool get_is_ducking() const;
    void set_ducking(bool ducking);
    bool is_on_item(ItemBox& box, SDL2pp::Renderer& renderer) const;
    bool get_is_grabbing() const;
    friend class StateManager; // Permite a StateManager acceder a miembros privados
};

#endif //CHARACTER_H

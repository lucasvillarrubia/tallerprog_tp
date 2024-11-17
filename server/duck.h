#ifndef DUCK_H
#define DUCK_H

#include "common/coordinates.h"

class Duck {
private:
    int id;
    float pos_X;
    float pos_Y;
    bool is_running;
    bool is_jumping;
    bool is_flapping;
    bool moving_right;
    bool is_on_the_floor;
    float jump_velocity;
public:
    // esto para el primer intento nomás
    // bool exited = false;
    Duck();
    void set_position(float, float);
    void set_position(const Coordinates& coordinates) {
        pos_X = coordinates.pos_X;
        pos_Y = coordinates.pos_Y;
    }
    void set_is_NOT_on_the_floor() {
        is_on_the_floor = false;
        // is_jumping = true;
    }
    void set_is_on_the_floor() {
        is_on_the_floor = true;
        is_jumping = false;
        jump_velocity = 0.0f;
    }
    void update_position(unsigned int);
    bool is_falling() const { return jump_velocity < 0.0f; };
    bool is_walking() const { return is_running; };
    bool is_moving_to_the_right() const { return moving_right; };
    bool is_on_the_ground() const { return is_on_the_floor; };
    // Coordinates get_coordinates() const;
    // int get_movement_phase () const;
    friend class StateManager;
};


#endif

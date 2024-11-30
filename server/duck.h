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
    bool is_stomping_on_wall_from_right;
    bool is_stomping_on_wall_from_left;
    bool is_alive;
    float jump_velocity;
    bool is_grabbing;
    bool is_shooting;
    bool with_gun;
    int gun_equipped_id;
    bool is_slipping;
    bool is_pointing_upwards;
    bool is_ducking;
public:
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
    void set_is_stomping_on_wall_from_right() {
        is_stomping_on_wall_from_right = true;
        is_running = false;
    }
    void set_is_NOT_stomping_on_wall_from_right() {
        is_stomping_on_wall_from_right = false;
    }
    void set_is_stomping_on_wall_from_left() {
        is_stomping_on_wall_from_left = true;
        is_running = false;
    }
    void set_is_NOT_stomping_on_wall_from_left() {
        is_stomping_on_wall_from_left = false;
    }
    void stop_flying() {
        jump_velocity = 0.0f;
    }
    void set_is_NOT_alive() {
        is_alive = false;
    }
    bool wants_to_grab();
    bool have_a_gun();
    void pickup_gun(int _gun_id);
    void stop_grab();
    void drop_gun();
    bool shooting();
    int get_gun_id() const;
    // Coordinates get_coordinates() const;
    // int get_movement_phase () const;
    friend class StateManager;
};


#endif

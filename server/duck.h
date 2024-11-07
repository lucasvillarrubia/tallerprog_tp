#ifndef DUCK_H
#define DUCK_H


class Duck {
private:
    int id;
    float pos_X;
    float pos_Y;
    bool is_running;
    bool is_jumping;
    bool is_flapping;
    bool moving_right;
    float jump_velocity;
public:
    // esto para el primer intento nomás
    // bool exited = false;
    Duck();
    bool update_position(unsigned int);
    // bool is_moving_to_the_right() const;
    // Coordinates get_coordinates() const;
    // int get_movement_phase () const;
    friend class StateManager;
};


#endif

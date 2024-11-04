#ifndef CHARACTER_H
#define CHARACTER_H


#include "coordinates.h"


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
public:
    Character();
    void update_position(unsigned int);
    bool is_moving_to_the_right() const;
    Coordinates get_coordinates() const;
    int get_movement_phase (unsigned int) const;
    friend class StateManager;
};


#endif //CHARACTER_H

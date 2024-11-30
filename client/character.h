#ifndef CHARACTER_H
#define CHARACTER_H


const float INITIAL_X_COORDINATE = 0.0f;
const float INITIAL_Y_COORDINATE = 0.0f;
// const float INITIAL_Y_COORDINATE = -85.0f;
const int AVAILABLE_MOVEMENT_SPRITES = 6;


struct Character {
    int id;
    float pos_X;
    float pos_Y;
    bool is_running;
    bool is_jumping;
    bool is_flapping;
    bool moving_right;
    bool is_alive;
    bool is_slipping;
    bool is_pointing_upwards;
    bool is_ducking;
    float jump_velocity;
    int movement_phase;
    explicit Character(const int _id):
        id(_id),
        pos_X(INITIAL_X_COORDINATE),
        pos_Y(INITIAL_Y_COORDINATE),
        is_running(false),
        is_jumping(false),
        is_flapping(false),
        moving_right(true),
        is_alive(true),
        is_slipping(false),
        is_pointing_upwards(false),
        is_ducking(false),
        jump_velocity(0.0f),
        movement_phase(0) {}
    // void update_position(unsigned int);
    // bool is_moving_to_the_right() const;
    // Coordinates get_coordinates() const;
    // int get_movement_phase (unsigned int) const;
    int get_movement_phase (const unsigned int frame_ticks) const {
        return (is_running ? (frame_ticks / AVAILABLE_MOVEMENT_SPRITES) % AVAILABLE_MOVEMENT_SPRITES : 0);
        // return (is_running ? ((frame_ticks / 100) % AVAILABLE_MOVEMENT_SPRITES) : 0);
    }
};


#endif //CHARACTER_H

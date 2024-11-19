#ifndef DUCK_H
#define DUCK_H


class Duck {
private:
    int id;
    float pos_X;
    float pos_Y;
    //bool is_alive;
    bool is_running;
    bool is_jumping;
    bool is_flapping;
    bool is_grabbing;
    bool is_shooting;
    bool with_gun;
    bool with_armor;
    bool with_helmet;
    bool moving_right;
    bool is_on_the_floor;
    float jump_velocity;
    int gun_id;
public:
    // esto para el primer intento nomás
    // bool exited = false;
    Duck();
    void set_position(float, float);
    void set_is_NOT_on_the_floor() {
        is_on_the_floor = false;
        // is_jumping = true;
    }
    void set_is_on_the_floor() {
        is_on_the_floor = true;
        // is_jumping = false;
        // jump_velocity = 0;
    }
    void update_position(unsigned int);
    bool is_moving_to_the_right() const;
    // Coordinates get_coordinates() const;
    // int get_movement_phase () const;
    bool wants_to_grab();
    bool have_a_gun();
    void pickup_gun(int _gun_id);
    void stop_grab();
    void drop_gun();
    bool shooting();
    int get_gun_id() const;
    friend class StateManager;
};


#endif

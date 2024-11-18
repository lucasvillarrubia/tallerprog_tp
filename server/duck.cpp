#include "duck.h"

#include <iostream>

const float INITIAL_X_COORDINATE = 0.0f;
const float INITIAL_Y_COORDINATE = 0.0f;
// const float INITIAL_Y_COORDINATE = -85.0f;
const float GRAVITY = -0.7f;
const float X_CONSTANT_VELOCITY = 5.0f;
const float TARGET_FRAME_RATE = 60.0f;
const float TARGET_FRAME_TIME = TARGET_FRAME_RATE / 1000.0f ;
const float GRAVITY_RESISTANCE_WHEN_FLAPPING = 0.1f;
const int AVAILABLE_MOVEMENT_SPRITES = 6;


Duck::Duck():
        pos_X(INITIAL_X_COORDINATE),
        pos_Y(INITIAL_Y_COORDINATE),
        is_running(false),
        is_jumping(false),
        is_flapping(false),
        moving_right(true),
        is_on_the_floor(false),
        jump_velocity(0.0f){}

void Duck::update_position(unsigned int frame_delta)
{
    if (is_running) {
        pos_X += X_CONSTANT_VELOCITY * (moving_right ? 1 : -1) ;
    }
    // Está saltando
    if (is_jumping or not is_on_the_floor) {
        float effective_gravity = GRAVITY;
        pos_Y += jump_velocity * (frame_delta * TARGET_FRAME_TIME);
        jump_velocity += effective_gravity * (frame_delta * TARGET_FRAME_TIME);
    }
    // Llegó al límite de la pantalla ---> Se mueve la cámara
    // if (pos_X > 640.0f)
    // {
    //     pos_X = -50.0f;
    // }
    // else if (pos_X < -50.0f)
    // {
    //     pos_X = 640.0f;
    // }
    // if (pos_Y < -64.0f)
    // {
    //     pos_Y = 544.0f;
    // }
}

void Duck::set_position(float x, float y) {
    pos_X = x;
    pos_Y = y;
}

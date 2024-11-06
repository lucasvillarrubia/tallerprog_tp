#include "duck.h"

#include <iostream>

const float INITIAL_X_COORDINATE = 0.0f;
const float INITIAL_Y_COORDINATE = 0.0f;
// const float INITIAL_Y_COORDINATE = -85.0f;
const float GRAVITY = -0.7f;
const float X_CONSTANT_VELOCITY = 5.0f;
const float TARGET_FRAME_RATE = 60.0f;
const float TARGET_FRAME_TIME = TARGET_FRAME_RATE / 1000.0f;
const float GRAVITY_RESISTANCE_WHEN_FLAPPING = 0.1f;
const int AVAILABLE_MOVEMENT_SPRITES = 6;


Duck::Duck():
        pos_X(INITIAL_X_COORDINATE),
        pos_Y(INITIAL_Y_COORDINATE),
        is_running(false),
        is_jumping(false),
        is_flapping(false),
        moving_right(true),
        jump_velocity(0.0f){}

void Duck::update_position(const unsigned int frame_delta)
{
    // Está corriendo (moviendose horizontalmente)
    if (is_running) {
        // pos_X += frame_delta * X_CONSTANT_VELOCITY * (moving_right ? 1 : -1);
        pos_X += X_CONSTANT_VELOCITY * (moving_right ? 1 : -1) ;
    }
    // Está saltando
    if (is_jumping) {
        float effective_gravity = (is_flapping && jump_velocity < 0) ? GRAVITY * GRAVITY_RESISTANCE_WHEN_FLAPPING : GRAVITY;
        pos_Y += jump_velocity * (frame_delta * TARGET_FRAME_TIME);
        // pos_Y += jump_velocity * (frame_delta);
        // pos_Y += jump_velocity;
        jump_velocity += effective_gravity * (frame_delta * TARGET_FRAME_TIME);
        // jump_velocity += effective_gravity * (frame_delta);
        // jump_velocity += effective_gravity;

        if (pos_Y <= INITIAL_Y_COORDINATE) {
            pos_Y = INITIAL_Y_COORDINATE;
            is_jumping = false;
            is_flapping = false;
        }
    }
    // Llegó al límite de la pantalla ---> Se mueve la cámara
    if (pos_X > 640.0f)
    {
        // std::cout << "se llegó al fin de la ventana???? posiciónX: " << pos_X << "\n";
        pos_X = -50.0f;
    }
    else if (pos_X < -50.0f)
        pos_X = 640.0f;
    // std::cout << "el pato está en las coordenadas x: " << pos_X << "; y: " << pos_Y << "\n";
}

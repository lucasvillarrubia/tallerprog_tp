#include "character.h"

#include <iostream>


const float INITIAL_X_COORDINATE = 0.0f;
const float INITIAL_Y_COORDINATE = 0.0f;
// const float INITIAL_Y_COORDINATE = -85.0f;
const float GRAVITY = -0.7f;
const float X_CONSTANT_VELOCITY = 0.2f;
const float TARGET_FRAME_RATE = 60.0f;
const float TARGET_FRAME_TIME = TARGET_FRAME_RATE / 1000.0f;
const float GRAVITY_RESISTANCE_WHEN_FLAPPING = 0.1f;
const int AVAILABLE_MOVEMENT_SPRITES = 6;


Character::Character():
        pos_X(INITIAL_X_COORDINATE),
        pos_Y(INITIAL_Y_COORDINATE),
        is_running(false),
        is_jumping(false),
        is_flapping(false),
        moving_right(true),
        jump_velocity(0.0f),
        movement_phase(0) {}

void Character::update_position(const unsigned int frame_delta)
{
    // std::cout << "veamos si cambia el render width: " << render_width << "\n";
    // Está corriendo (moviendose horizontalmente)
    if (is_running)
        pos_X += frame_delta * X_CONSTANT_VELOCITY * (moving_right ? 1 : -1);
    // Está saltando
    if (is_jumping) {
        float effective_gravity = (is_flapping && jump_velocity < 0) ? GRAVITY * GRAVITY_RESISTANCE_WHEN_FLAPPING : GRAVITY;
        pos_Y += jump_velocity * (frame_delta * TARGET_FRAME_TIME);
        jump_velocity += effective_gravity * (frame_delta * TARGET_FRAME_TIME);

        if (pos_Y <= INITIAL_Y_COORDINATE) {
            pos_Y = INITIAL_Y_COORDINATE;
            is_jumping = false;
            is_flapping = false;
        }
    }
    // Llegó al límite de la pantalla ---> Se mueve la cámara
    // if (pos_X > 640)
    //     pos_X = -50;
    // else if (pos_X < -50)
    //     pos_X = 640;
}

bool Character::is_moving_to_the_right() const { return moving_right; }

Coordinates Character::get_coordinates() const { return {pos_X, pos_Y}; }

int Character::get_movement_phase (const unsigned int frame_ticks) const {
    return (is_running ? ((frame_ticks / 100) % AVAILABLE_MOVEMENT_SPRITES) : 0);
}

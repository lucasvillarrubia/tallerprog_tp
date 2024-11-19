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
        //is_alive(true),
        is_running(false),
        is_jumping(false),
        is_flapping(false),
        is_grabbing(false),
        is_shooting(false),
        with_gun(false),
    	with_armor(false),
    	with_helmet(false),
        moving_right(true),
        is_on_the_floor(false),
        jump_velocity(0.0f){}

void Duck::update_position(unsigned int frame_delta)
{
    if (is_on_the_floor)
    {
        is_jumping = false;
        // is_flapping = false;
        frame_delta++;
        jump_velocity = 0;
    }
    if (not is_on_the_floor) {
        is_jumping = true;
        // is_flapping = false;
    }
    if (is_running) {
        pos_X += X_CONSTANT_VELOCITY * (moving_right ? 1 : -1) ;
    }
    // Está saltando
    if (is_jumping) {
        is_on_the_floor = false;
        float effective_gravity = GRAVITY;
        pos_Y += jump_velocity * (frame_delta * TARGET_FRAME_TIME);
        jump_velocity += effective_gravity * (frame_delta * TARGET_FRAME_TIME);
    }
    // Llegó al límite de la pantalla ---> Se mueve la cámara
    if (pos_X > 640.0f)
    {
        pos_X = -50.0f;
    }
    else if (pos_X < -50.0f)
    {
        pos_X = 640.0f;
    }
    if (pos_Y < -64.0f)
    {
        pos_Y = 544.0f;
    }
}

void Duck::set_position(float x, float y) {
    pos_X = x;
    pos_Y = y;
}

bool Duck::wants_to_grab() {
	return is_grabbing;
}

bool Duck::have_a_gun() {
	return with_gun;
}

void Duck::pickup_gun() {
	with_gun = true;
}

void Duck::stop_grab() {
	is_grabbing = false;
}

void Duck::drop_gun() {
	with_gun = false;
}

bool Duck::shooting() {
	return is_shooting;
}

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
        is_stomping_on_wall_from_right(false),
        is_stomping_on_wall_from_left(false),
        is_alive(true),
        jump_velocity(0.0f),
        is_grabbing(false),
        is_shooting(false),
        with_gun(false),
        gun_equipped_id(0){}

void Duck::update_position(unsigned int frame_delta)
{
    if (is_running) {
        pos_X += X_CONSTANT_VELOCITY * (moving_right ? 1 : -1) ;
    }
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
    if (pos_Y < -400.0f)
    {
        set_is_NOT_alive();
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

void Duck::pickup_gun(int _gun_id) {
	gun_equipped_id = _gun_id;
	with_gun = true;
}

void Duck::stop_grab() {
	is_grabbing = false;
}

void Duck::drop_gun() {
	gun_equipped_id = 0;
	with_gun = false;
}

bool Duck::shooting() {
	return is_shooting;
}

int Duck::get_gun_id() const{
	return gun_equipped_id;
}

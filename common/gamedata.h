#ifndef GAMEDATA_H
#define GAMEDATA_H
#include <list>
#include <map>
#include <utility>

#include "coordinates.h"


struct Gamedata {
    int player_id;
    explicit Gamedata(const int player): player_id(player) {}
};

struct Gamestate: Gamedata {
    int type;
    int gun_id;
    int is_bullet_data;
    int bullet_id;
    int type_gun;
    float pos_X;
    float pos_Y;
    int is_running;
    int is_jumping;
    int is_flapping;
    int is_grabbing;
    int is_shooting;
    int with_gun;
    int with_armor;
    int with_helmet;
    int move_direction;
    float jump_speed;
    std::map<int, Coordinates> positions_by_id;
    std::map<int,std::pair<int, Coordinates>> guns_positions_by_type;
    std::map<int,std::pair<int, Coordinates>> bullets_positions_by_type;
    //int is_picked_up;
    Gamestate(): Gamedata(-1) {}
    Gamestate(
        const int player,
        const float x,
        const float y,
        const int run,
        const int jump,
        const int flap,
        const int grab,
        const int shoot,
        const int gun,
        const int armor,
        const int helmet,
        const int direction,
        const float jumpspeed
    ):
        Gamedata(player),
        type(1),
        pos_X(x),
        pos_Y(y),
        is_running(run),
        is_jumping(jump),
        is_flapping(flap),
        is_grabbing(grab),
        is_shooting(shoot),
        with_gun(gun),
        with_armor(armor),
        with_helmet(helmet),
        move_direction(direction),
        jump_speed(jumpspeed) {}
    Gamestate(
        const int player,
        const int run,
        const int jump,
        const int flap,
        const int grab,
        const int shoot,
        const int gun,
        const int armor,
        const int helmet,
        const int direction
        ):
        Gamedata(player),
        type(3),
        is_running(run),
        is_jumping(jump),
        is_flapping(flap),
        is_grabbing(grab),
        is_shooting(shoot),
        with_gun(gun),
        with_armor(armor),
        with_helmet(helmet),
        move_direction(direction) {}
    Gamestate(std::map<int,Coordinates>& positions):
        Gamedata(0),
        type(2),
        positions_by_id(positions) {}
	Gamestate(std::map<int,std::pair<int, Coordinates>> guns_positions):
		Gamedata(0),
		type(4),
        guns_positions_by_type(guns_positions) {}
    Gamestate(const int _id, const int _type_gun, const float x, const float y) :
    	Gamedata(0),
		type(5),
        gun_id(_id),
        type_gun(_type_gun),
        pos_X(x),
        pos_Y(y) {}
    Gamestate(const int flag, const int _id, const int _type_gun, const float x, const float y) :
    	Gamedata(0),
    	type(6),
    	is_bullet_data(flag),
    	bullet_id(_id),
    	type_gun(_type_gun),
    	pos_X(x),
    	pos_Y(y) {}
    Gamestate(const int flag, std::map<int,std::pair<int, Coordinates>> bullets_positions) :
    	Gamedata(0),
    	type(7),
    	is_bullet_data(flag),
    	bullets_positions_by_type(bullets_positions) {}
    Gamestate(const int flag, const int id) :
    	Gamedata(0),
    	type(8),
    	is_bullet_data(flag),
    	bullet_id(id)
    	{}
};

struct Gameaction: Gamedata {
    int type;
    int key;
    Gameaction(): Gamedata(-1) {}
    Gameaction(const int player, const int _type, const int _key): Gamedata(player), type(_type), key(_key) {}
};


#endif

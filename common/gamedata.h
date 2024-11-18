#ifndef GAMEDATA_H
#define GAMEDATA_H
#include <list>
#include <map>
#include <string>

#include "coordinates.h"


struct Gamedata {
    int player_id;
    explicit Gamedata(const int player): player_id(player) {}
};

struct Gamestate: Gamedata {
    int type;
    float pos_X;
    float pos_Y;
    int is_running;
    int is_jumping;
    int is_flapping;
    int move_direction;
    float jump_speed;
    std::map<int, Coordinates> positions_by_id;
    std::string message;

    Gamestate(): Gamedata(-1) {}

    Gamestate(
        const int player,
        const float x,
        const float y,
        const int run,
        const int jump,
        const int flap,
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
        move_direction(direction),
        jump_speed(jumpspeed) {}

    Gamestate(
        const int player,
        const int run,
        const int jump,
        const int flap,
        const int direction
        ):
        Gamedata(player),
        type(3),
        is_running(run),
        is_jumping(jump),
        is_flapping(flap),
        move_direction(direction) {}

    Gamestate(std::map<int,Coordinates>& positions):
        Gamedata(0),
        type(2),
        positions_by_id(positions) {}

    Gamestate(const int player, const std::string& msg): Gamedata(player), type(4), message(msg) {}
};

struct Gameaction: Gamedata {
    int match;
    int type;
    int key;
    bool is_multiplayer;
    Gameaction(): Gamedata(-1) {}
    Gameaction(
        const int player,
        const int _match,
        const int _type,
        const int _key
        ):
        Gamedata(player),
        match(_match),
        type(_type),
        key(_key) {}
    Gameaction(const int player, const int _type, const bool mode): Gamedata(player), type(_type), is_multiplayer(mode) {}
};


#endif

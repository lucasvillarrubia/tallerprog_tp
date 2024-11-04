#ifndef GAMEDATA_H
#define GAMEDATA_H


struct Gamedata {
    int player_id;
    explicit Gamedata(const int player): player_id(player) {}
};

struct Gamestate: Gamedata {
    float pos_X;
    float pos_Y;
    int is_running;
    int is_jumping;
    int is_flapping;
    int move_direction;
    float jump_speed;
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
    pos_X(x),
    pos_Y(y),
    is_running(run),
    is_jumping(jump),
    is_flapping(flap),
    move_direction(direction),
    jump_speed(jumpspeed){}
};

struct Gameaction: Gamedata {
    int type;
    int key;
    Gameaction(): Gamedata(-1) {}
    Gameaction(const int player, const int _type, const int _key): Gamedata(player), type(_type), key(_key) {}
};


#endif

#ifndef GAMEDATA_H
#define GAMEDATA_H


struct Gamedata {
    int player_id;
    int type;
    int key;
    Gamedata(const int player, const int _type, const int _key): player_id(player), type(_type), key(_key) {}
};

struct Gamestate: Gamedata {
    // float jump_speed;
    Gamestate(): Gamedata(0, 0, 0) {}
    Gamestate(const int player, const int _type, const int _key): Gamedata(player, _type, _key) {}
};

struct Gameaction: Gamedata {
    Gameaction(): Gamedata(0, 0, 0) {}
    Gameaction(const int player, const int _type, const int _key): Gamedata(player, _type, _key) {}
};


#endif

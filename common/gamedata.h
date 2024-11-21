#ifndef GAMEDATA_H
#define GAMEDATA_H
#include <list>
#include <map>
#include <string>

#include "coordinates.h"
#include "drawingdata.h"


struct Gamedata {
    int player_id;
    explicit Gamedata(const int player): player_id(player) {}
};

struct Gamestate: Gamedata {
    int type;
    int object_id;
    float pos_X;
    float pos_Y;
    int is_running;
    int is_jumping;
    int is_flapping;
    int type_gun;
    int move_direction;
    int is_alive;
    float jump_speed;
    std::map<int, Coordinates> positions_by_id;
    std::map<int, std::pair<DrawingData, Coordinates>> guns_positions_by_id;
    int bullet_flag;
    std::map<int, Coordinates> bullets_positions_by_id;
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
        const int life,
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
        is_alive(life),
        jump_speed(jumpspeed) {}

    Gamestate(
        const int player,
        const int run,
        const int jump,
        const int flap,
        const int direction,
        const int life
        ):
        Gamedata(player),
        type(3),
        is_running(run),
        is_jumping(jump),
        is_flapping(flap),
        move_direction(direction),
        is_alive(life) {}

    Gamestate(std::map<int,Coordinates>& positions):
        Gamedata(0),
        type(2),
        positions_by_id(positions) {}

    Gamestate(const int player, const std::string& msg): Gamedata(player), type(4), message(msg) {}
    
    //envia la inicializacion de un arma
    Gamestate(
    	const int _id,
    	float x,
    	float y,
    	const int _type,
    	const int direction
    ) :
    	Gamedata(0),
    	type(5),
    	object_id(_id),
        pos_X(x),
        pos_Y(y),
        type_gun(_type),
    	move_direction(direction) {}
	
	//envia las nuevas posiciones de las armas
	Gamestate(std::map<int, std::pair<DrawingData, Coordinates>>& guns_positions) :
		Gamedata(0),
    	type(6),
    	guns_positions_by_id(guns_positions){}
    
    //envia la inicializacion de una bala
    Gamestate(
    	const int _id,
    	const int _type,
    	const int direction,
    	float x,
    	float y
    ) : 
    	Gamedata(0),
    	type(7),
    	object_id(_id),
        pos_X(x),
        pos_Y(y),
        type_gun(_type),
    	move_direction(direction) {}
    
    //envia las nuevas posiciones de las balas--el flag es para que no se confunda con el del update de posiciones de patos--
    Gamestate(
    	int flag,
    	std::map<int, Coordinates>& bullets_positions
    ) : 
		Gamedata(0),
    	type(8),
    	bullet_flag(flag),
    	bullets_positions_by_id(bullets_positions)
    	{}
    
    //envia la señal de bala destruida
    Gamestate(const int _id) : 
		Gamedata(0),
    	type(9),
    	object_id(_id){}
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

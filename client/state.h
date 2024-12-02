#ifndef STATE_H
#define STATE_H


#include <list>
#include "character.h"
#include "gun.h"
#include "bullet.h"


struct State {
    const std::list<Character> dukis;
    const std::list<Gun> guns;
    const std::list<Bullet> bullets;
    State(const std::list<Character>& d, const std::list<Gun>& g, const std::list<Bullet>& b):
        dukis(d),
        guns(g),
        bullets(b) {}
};


#endif

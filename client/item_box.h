#ifndef ITEM_BOX_H
#define ITEM_BOX_H

#include "SDL2pp/SDL2pp.hh"
#include "coordinates.h"

class ItemBox {
private:
    Coordinates position;
    SDL2pp::Texture texture;
    bool collected;
public:

    ItemBox(SDL2pp::Renderer& renderer, const std::string& texturePath, float posX, float posY);
    void render(SDL2pp::Renderer& renderer);
    Coordinates get_position() const;

    bool is_collected();

    void set_collected(bool value);

    };

#endif // ITEM_BOX_H
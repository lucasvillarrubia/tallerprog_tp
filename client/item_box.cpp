#include "item_box.h"

ItemBox::ItemBox(SDL2pp::Renderer& renderer, const std::string& texturePath, float posX, float posY)
        : position{posX, posY},  texture(renderer, texturePath), collected(false) // Inicializa la textura
{
}

void ItemBox::render(SDL2pp::Renderer& renderer) {

    if (collected) {
        // No renderiza la caja si ha sido recogida
        return;
    }

    SDL2pp::Rect dst_rect(static_cast<int>(position.pos_X),
                          static_cast<int>(renderer.GetOutputHeight() / 2 - 32 - position.pos_Y),
                          64, 64);

    // Define la fuente de la textura (asumiendo que quieres mostrar toda la textura)
    SDL2pp::Rect src_rect(0, 0, texture.GetWidth(), texture.GetHeight());

    // Llama a Copy usando Optional<Rect>
    renderer.Copy(texture, src_rect, dst_rect); // Dibuja la caja
}

bool ItemBox::is_collected() {
    return collected;
}

void ItemBox::set_collected(bool value) {
    collected = value;
}

Coordinates ItemBox::get_position() const {
    return position; // Devuelve la posición de la caja
}
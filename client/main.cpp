#include "client.h"
#include "coordinates.h"
#include "character.h"
#include "state_manager.h"


#include <iostream>
#include <exception>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <SDL_image.h>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/Texture.hh>
#include <SDL2pp/Surface.hh>
#include "mapa.h"
#include "item_box.h"
#include "Magnum.h"
#include "MagnumAmmo.h"
#include "ammo.h"

using namespace SDL2pp;

const int DUCK_SPRITE_WIDTH = 64;
const int DUCK_SPRITE_HEIGHT = 64;
const int DUCK_MOVEMENT_SPRITES_LINE = 0;


int main() try {
    SDL2pp::SDL sdl(SDL_INIT_VIDEO);
    SDL2pp::Window window(
            "Duck Game",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            640, 480,
            SDL_WINDOW_RESIZABLE
    );
    SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    Mapa mapa(renderer, "resources/mapas/fondo_pantano.png");

    SDL2pp::Surface tempSurface("resources/Duck-removebg-preview.png");
    SDL2pp::Texture sprites(renderer, tempSurface);

    SDL2pp::Surface duckingSurface("resources/pato_ducking.png");
    SDL2pp::Texture duckingSprites(renderer, duckingSurface);

    SDL2pp::Surface pistolSurface("resources/PC Computer - Duck Game - Pistol.png");
    SDL2pp::Texture pistolSprites(renderer, pistolSurface);

    std::vector<ItemBox> boxes;
    boxes.emplace_back(renderer, "resources/box.png", 320.0f, -80.0f); // Primera caja
    boxes.emplace_back(renderer, "resources/box.png", 200.0f, -50.0f); // Segunda caja


    unsigned int prev_ticks = SDL_GetTicks();
    Character duck;
    Magnum magnum(50, -85);
    std::vector<MagnumAmmo> bullets;

    while (true) {
        unsigned int frame_ticks = SDL_GetTicks();
        unsigned int frame_delta = frame_ticks - prev_ticks;
        prev_ticks = frame_ticks;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                return 0;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    return 0;
            }
            StateManager::update_duck_state(duck, event);
        }

        duck.update_position(frame_delta, frame_ticks, renderer.GetOutputWidth());

	//actualizo la posicion de las balas
	for (size_t i=0; i<bullets.size(); i++){
	    if (!bullets[i].isDestroyed()) { 
		bullets[i].updatePosition(frame_delta);
	    } else {
	    	bullets.erase(bullets.begin()+i);
	    }
	    //las balas que alcanzaron su distancia límite no se actualizan
	}

        // Limpiar y dibujar el mapa
        renderer.Clear();
        mapa.dibujar_fondo(renderer); // Dibuja el fondo del mapa

        int src_x;
        int src_y;
        Coordinates duck_position = duck.get_coordinates();
        SDL_RendererFlip flip = duck.is_moving_to_the_right() ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

        if (duck.get_is_ducking()) {
            src_x = 0;
            src_y = 0;
            SDL_Rect dst_rect = { static_cast<int>(duck_position.pos_X),
                                  static_cast<int>(renderer.GetOutputHeight() / 2 - 32 - duck_position.pos_Y),
                                  64, 64 };

            SDL_RenderCopyEx(renderer.Get(), duckingSprites.Get(), nullptr, &dst_rect, 0.0, nullptr, flip);
        } else {
            src_x = DUCK_SPRITE_WIDTH * duck.get_movement_phase();
            src_y = DUCK_MOVEMENT_SPRITES_LINE;

            SDL_Rect src_rect = { src_x, src_y, DUCK_SPRITE_WIDTH, DUCK_SPRITE_HEIGHT };
            SDL_Rect dst_rect = { static_cast<int>(duck_position.pos_X),
                                  static_cast<int>(renderer.GetOutputHeight() / 2 - 32 - duck_position.pos_Y),
                                  64, 64 };
            SDL_RenderCopyEx(renderer.Get(), sprites.Get(), &src_rect, &dst_rect, 0.0, nullptr, flip);
        }
        
        
        //intento imprimir el arma
        if (magnum.isPickedUp()) {
        	//seteo la posicion del arma segun la orientación del pato
        	float gun_position_x = duck.is_moving_to_the_right() ? duck_position.pos_X+16 : duck_position.pos_X;
        	magnum.updatePosition(gun_position_x, duck_position.pos_Y);
        	magnum.updateDirection(duck.is_moving_to_the_right());
        }       
        //ubicacion del sprite en el png y sus dimensiones
	SDL_Rect src_rect = { 1, 47, 32, 32 };
        SDL_Rect dst_rect = { static_cast<int>(magnum.getPosition().pos_X),
                              static_cast<int>(renderer.GetOutputHeight() / 2 - 16 - magnum.getPosition().pos_Y),
                                  48, 48 };
	SDL_RendererFlip gunFlip = magnum.isPickedUp() ? flip : SDL_FLIP_NONE;
        SDL_RenderCopyEx(renderer.Get(), pistolSprites.Get(), &src_rect, &dst_rect, 0.0, nullptr, gunFlip);
            
        //Verifico colisión con el arma
        if (!magnum.isPickedUp() && duck.is_on_gun(magnum, renderer) && duck.get_is_grabbing()) {
        	magnum.collected();
        }
        
        //realizo el disparo
	if (duck.get_is_shooting()) {
		MagnumAmmo bullet = magnum.shoot();
            	if (!bullet.isDestroyed()) {
            		bullets.push_back(bullet);
            	}
	}else{
		magnum.stopShoot();
	}
	//intento imprimir todas las balas del escenario
	for (size_t i=0; i<bullets.size(); i++){
		if (bullets[i].isDestroyed()){
			continue; //si las balas llegaron a su alcance limite no se imprimen
		}
		SDL_Rect src_bullet_rect = { 1, 89, 16, 16 };
        	SDL_Rect dst_bullet_rect = { static_cast<int>(bullets[i].getPosition().pos_X),
                              static_cast<int>(renderer.GetOutputHeight() / 2 - 8 - bullets[i].getPosition().pos_Y),
                                  24, 24 };
	SDL_RendererFlip bulletFlip = bullets[i].isMovingRight() ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
        SDL_RenderCopyEx(renderer.Get(), pistolSprites.Get(), &src_bullet_rect, &dst_bullet_rect, 0.0, nullptr, bulletFlip);
	}
        
        // Verificar colisión con cada caja de premios en el vector
        /*for (auto& box : boxes) {
            if (!box.is_collected() && duck.is_on_item(box, renderer) && duck.get_is_grabbing()) {
                box.set_collected(true);
            }
            // Renderizar la caja si no ha sido recogida
            if (!box.is_collected()) {
                box.render(renderer);
            }
        }*/
        // comentado para poder ver mejor por pantalla a las balas



        renderer.Present();
        SDL_Delay(1);
    }
} catch (std::exception& e) {
    std::cerr << "Error in client main. " << e.what() << "\n";
    return -1;
}


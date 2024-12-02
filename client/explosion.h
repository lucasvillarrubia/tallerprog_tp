#ifndef EXPLOSION_H
#define EXPLOSION_H

struct Explosion {
	float pos_X;
	float pos_Y;
	int previous_state;
	int current_state;
	bool end;
	
	Explosion(float x, float y) :
		pos_X(x),
		pos_Y(y),
		previous_state(0),
		current_state(0),
		end(false) {}
	
	void set_next_phase(const unsigned int frame_ticks) {
		previous_state = current_state;
		current_state = (frame_ticks / 6) % 6;
		if (previous_state > current_state) {
			end = true;
			current_state = 0;
		}
	}
};

#endif

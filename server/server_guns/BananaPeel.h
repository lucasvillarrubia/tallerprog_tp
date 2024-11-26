#ifndef BANANAPEEL_H
#define BANANAPEEL_H

class BananaPeel {
private:
	bool is_the_trap_set;
public:
	BananaPeel(float x, float y, bool right);
	void update_position(const unsigned int frame_delta);
	void make_the_duck_slip(Duck& duck);
	void trap_set();
	bool is_in_the_ground();
};

#endif

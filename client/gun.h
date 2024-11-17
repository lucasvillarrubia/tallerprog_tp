#ifndef GUN_H
#define GUN_H

struct Gun {
	int id;
	int type;
	float pos_X;
	float pos_Y;
	//bool shooting;
	//bool picked_up;
	bool moving_right;
	
	Gun(const int _id, const int _type, float x, float y): 
		id(_id),
		type(_type),
		pos_X(x),
		pos_Y(y),
		/*shooting(false),
		picked_up(false),*/
		moving_right(true) {}
};

#endif

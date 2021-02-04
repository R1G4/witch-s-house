#pragma once
#include "firstFloorStage.h"

class hallway : public firstFloorStage
{
	enum TRIGGER
	{
		NONE = 0,
		DOOR_LEFT_OPEN = 976,
		DOOR_RIGHT_TOP_OPEN = 561,
		DOOR_RIGHT_BOTTOM_OPEN = 981,
		CLOCK = 498
	};

	TRIGGER _trigger;
public:
	hallway();
	~hallway();

	HRESULT init(CHRDIRECTION _chrdirection = CHRDIREC_RIGHT, LOCATION _location = LOCATION_DEFAULT);
	void release();
	void update();
	void render();
	void Collision();
	void load(LOCATION location = LOCATION_DEFAULT);
};


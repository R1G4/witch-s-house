#pragma once
#include "firstFloorStage.h"

class hallway : public firstFloorStage
{
	enum TRIGGER
	{
		NONE,
		DOOR_LEFT_CLOSE ,
		DOOR_LEFT_OPEN,
		DOOR_RIGHT_TOP_CLOSE,
		DOOR_RIGHT_TOP_OPEN,
		DOOR_RIGHT_BOTTOM_CLOSE,
		DOOR_RIGHT_BOTTOM_OPEN
	};

	TRIGGER _trigger;
public:
	hallway();
	~hallway();

	HRESULT init();
	void release();
	void update();
	void render();
	void Collision();
	void load();
};


#pragma once
#include "firstFloorStage.h"

class scissorsRoom : public firstFloorStage
{
	enum TRIGGER
	{
		NONE = 0,
		DOOR_RIGHT_OPEN = 504,
		DOOR_UP_OPEN = 379,
		BEAR_PUT,
		PALM = 501,
		SCISSORS = 618,
		BEARCOM = 500,
		DELAY = 10000
	};

	TRIGGER _trigger;
public:
	scissorsRoom();
	~scissorsRoom();

	HRESULT init(CHRDIRECTION _chrdirection = CHRDIREC_LEFT, LOCATION _location = LOCATION_DEFAULT);
	void release();
	void update();
	void render();
	void Collision();
	void load();
	void getMemory();
};

#pragma once
#include "firstFloorStage.h"

class entrance : public firstFloorStage
{
	enum TRIGGER
	{
		NONE = 0,
		DOOR_LEFT_OPEN = 496,
		DOOR_RIGHT_OPEN = 508,
		CAT_TALK,
		CANDLE_OFF = 442,
		VASE_DOWN = 499,
		DELAY
	};

	TRIGGER _trigger;
public:
	entrance();
	~entrance();

	HRESULT init(CHRDIRECTION _chrdirection = CHRDIREC_DOWN, LOCATION _location = LOCATION_DEFAULT);
	void release();
	void getMemory();
	void update();
	void render();
	void Collision();
	void load(LOCATION location = LOCATION_DEFAULT);
};


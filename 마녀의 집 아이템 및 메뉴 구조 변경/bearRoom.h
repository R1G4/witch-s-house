#pragma once
#include "firstFloorStage.h"

class bearRoom : public firstFloorStage
{
	enum TRIGGER
	{
		NONE = 0,
		DOOR_LEFT_OPEN =496,
		BEAR_PUT = 501
	};

	TRIGGER _trigger;
public:
	bearRoom();
	~bearRoom();

	HRESULT init(CHRDIRECTION _chrdirection = CHRDIREC_RIGHT, LOCATION _location = LOCATION_DEFAULT);
	void release();
	void update();
	void render();
	void Collision();
	void load();
	void getMemory();
};


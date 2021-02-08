#pragma once
#include "firstFloorStage.h"

class entranceFake : public firstFloorStage
{
	enum TRIGGER
	{
		NONE,
		DOOR_OPEN
	};

	TRIGGER _trigger;
	
public:
	entranceFake();
	~entranceFake();

	HRESULT init(CHRDIRECTION _chrdirection = CHRDIREC_UP, LOCATION _location = LOCATION_DEFAULT);
	HRESULT init();
	void release();
	void Collision();
	void load(LOCATION location = LOCATION_DEFAULT);
	void update();
	void render();
	//void load();
};
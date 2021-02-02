#pragma once
#include "firstFloorStage.h"

class entranceFake : public firstFloorStage
{
	enum TRIGGER
	{
		NONE,
		DOOR_CLOSE,
		DOOR_OPEN
	};

	TRIGGER _trigger;
	
public:
	entranceFake();
	~entranceFake();

	HRESULT init();
	void release();
	void Collision();
	void update();
	void render();
	void load();
};
#pragma once
#include "firstFloorStage.h"

class entranceTrap : public firstFloorStage
{
	enum TRIGGER
	{
		NONE = 0,
		DOOR_CLOSE = 1,
		DOOR_OPEN = 681,
		READ = 381
	};

	struct tagMission
	{
		MISSION read = FAIL;
	};

	TRIGGER _trigger;
	tagMission _mission;
public:
	entranceTrap();
	~entranceTrap();

	HRESULT init();
	void release();
	void update();
	void render();
	void Collision();
	void load();
};


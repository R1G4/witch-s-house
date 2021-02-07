#pragma once
#include "firstFloorStage.h"

class entranceTrap : public firstFloorStage
{
	enum TRIGGER
	{
		NONE = 0,
		DOOR_OPEN = 681,
		READ = 381,
		WALL
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

	HRESULT init(CHRDIRECTION _chrdirection = CHRDIREC_UP, LOCATION _location = LOCATION_DEFAULT);
	void release();
	void update();
	void render();
	void Collision();
	void load(LOCATION location = LOCATION_DEFAULT);
};


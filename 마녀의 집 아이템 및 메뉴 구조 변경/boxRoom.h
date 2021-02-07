#pragma once
#include "firstFloorStage.h"

class boxRoom : public firstFloorStage
{
	enum TRIGGER
	{
		NONE = 0,
		DOOR_LEFT_OPEN = 496,
		BEAR_PICKUP = 624,
		BOX = 560,
		READ = 381,
		THORN = 384,
		DELAY
	};

	enum READCNT
	{
		FIRST,
		SECOND,
		THIRD,
		FOURTH,
		END
	};

	TRIGGER _trigger;
	READCNT _readCnt;
public:
	boxRoom();
	~boxRoom();

	HRESULT init(CHRDIRECTION _chrdirection = CHRDIREC_RIGHT, LOCATION _location = LOCATION_DEFAULT);
	void release();
	void update();
	void render();
	void Collision();
	void load();
	void getMemory();
};


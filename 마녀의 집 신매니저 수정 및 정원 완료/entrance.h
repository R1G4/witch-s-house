#pragma once
#include "firstFloorStage.h"

class entrance : public firstFloorStage
{
	enum TRIGGER
	{
		NONE = 0,
		DOOR_LEFT_CLOSE = 1,
		DOOR_LEFT_OPEN,
		DOOR_RIGHT_CLOSE = 2,
		DOOR_RIGHT_OPEN,
		CAT_TALK,
		CANDLE_OFF = 442,
		VASE_DOWN = 499
	};
	/*¿Ã∞« ∫πµµ ∏ 
	enum TRIGGER
	{
		NONE,
		DOOR_LEFT_CLOSE ,
		DOOR_LEFT_OPEN,
		DOOR_RIGHT_TOP_CLOSE,
		DOOR_RIGHT_TOP_OPEN,
		DOOR_RIGHT_BOTTOM_CLOSE,
		DOOR_RIGHT_BOTTOM_OPEN
	};*/

	TRIGGER _trigger;
public:
	entrance();
	~entrance();

	HRESULT init();
	void release();
	void update();
	void render();
	void Collision();
	void load();
};


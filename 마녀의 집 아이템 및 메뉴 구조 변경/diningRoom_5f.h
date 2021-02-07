#pragma once
#include "fifthFloorStage.h"

class diningRoom_5f : public fifthFloorStage
{
	enum CLICKORDER
	{
		FIRST,
		SECOND
	};

	enum TRIGGER
	{
		DOORTOGARDEN = 34 + 14 * TILEX,
		DOORTODININGROOMITEM = 29 + 19 * TILEX,
		DROPFLOWER = 22 + 13 * TILEX,
		Y_FLOWER_1 = 27 + 12 * TILEX,
		Y_FLOWER_2 = 28 + 14 * TILEX,
		Y_FLOWER_3 = 26 + 16 * TILEX,
		Y_FLOWER_4 = 24 + 14 * TILEX,
		CLOCK = 26 + 7 * TILEX,
		LIGHT = 17 + 14 * TILEX
	};

	TRIGGER _trigger;
	CLICKORDER _co;

	float _x, _y;
	FloatRect _rc;
	FloatRect _correct_rc;
	bool _isClick;
	bool _isClock;

public:
	diningRoom_5f() {};
	~diningRoom_5f() {};

	HRESULT init(CHRDIRECTION _chrdirection = CHRDIREC_UP, LOCATION _location = LOCATION_DEFAULT);
	void release();
	void update();
	void render();
	void Collision();
	void load(LOCATION location = LOCATION_DEFAULT);

	void setTrigger();
	void setChoiceScene();
	void getMemory();
};


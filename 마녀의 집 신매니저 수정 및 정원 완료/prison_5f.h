#pragma once
#include "fifthFloorStage.h"
class prison_5f : public fifthFloorStage
{
private:
	enum TRIGGER
	{
		DOORTOGARDEN = 17 + 15 * TILEX,
		DOORTOPRISONWELL = 22 + 4 * TILEX,
		DOORTOPRISONSKUL = 26 + 6 * TILEX,
		CAGE = 40 + 15 * TILEX,
		R_FLOWER_1 = 44 + 14 * TILEX,
		R_FLOWER_2 = 45 + 15 * TILEX,
		R_FLOWER_3 = 44 + 16 * TILEX,
		BOOK = 25 + 14 * TILEX,
		DOOR_1 = 30 + 14 * TILEX,
		DOOR_2 = 37 + 14 * TILEX,
		PAPER = 35 + 14 * TILEX,
		SKUL = 23 + 13 * TILEX
	};

	TRIGGER _trigger;
public:
	prison_5f() {};
	~prison_5f() {};

	HRESULT init();
	void release();
	void update();
	void render();
	void Collision();
	void load();

	void setTrigger();
};


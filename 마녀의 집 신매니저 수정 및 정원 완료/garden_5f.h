#pragma once
#include "fifthFloorStage.h"

class garden_5f : public fifthFloorStage
{
private:
	enum TRIGGER
	{
		DOORTO4F = 21 + 6 * TILEX,
		DOORTODININGROOM = 16 + 16 * TILEX,
		DOORTOPRISON = 30 + 16 * TILEX,
		DOORTOGARDENTOBOSS = 23 + 6 * TILEX,
		TREE = 22 + 14 * TILEX,
		FLOWER = 23 + 15 * TILEX,
		NEKO = 23 + 18 * TILEX
	};

	TRIGGER _trigger;
public:
	garden_5f() {};
	~garden_5f() {};

	HRESULT init();
	void release();
	void update();
	void render();
	void Collision();
	void load();

	void setTrigger();
};


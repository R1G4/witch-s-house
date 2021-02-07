#pragma once
#include "fifthFloorStage.h"

class garden_5f : public fifthFloorStage
{
private:
	enum CLICKORDER
	{
		FIRST,
		SECOND
	};

	enum TRIGGER
	{
		DOORTO4F = 21 + 6 * TILEX,
		DOORTODININGROOM = 16 + 16 * TILEX,
		DOORTOPRISON = 30 + 16 * TILEX,
		DOORTOGARDENTOBOSS = 23 + 5 * TILEX,
		TREE = 22 + 14 * TILEX,
		FLOWER = 23 + 15 * TILEX,
		NEKO = 23 + 18 * TILEX,
		TEE = 24 + 18 * TILEX,
	};

	TRIGGER _trigger;
	CLICKORDER _co;

	int _count;
	int _frame;

	float _x, _y;
	FloatRect _rc;
	FloatRect _correct_rc;

	bool _isClick;
	bool _deadFlower;
	bool _deadFlowerText;

public:
	garden_5f() {};
	~garden_5f() {};

	HRESULT init(CHRDIRECTION _chrdirection = CHRDIREC_UP, LOCATION _location = LOCATION_DEFAULT);
	void release();
	void update();
	void render();
	void Collision();
	void load(LOCATION location = LOCATION_DEFAULT);

	void setTrigger();
	void setChoiceScene();
};


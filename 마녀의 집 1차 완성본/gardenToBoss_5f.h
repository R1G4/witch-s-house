#pragma once
#include "fifthFloorStage.h"

class gardenToBoss_5f : public fifthFloorStage
{
private:
	enum TRIGGER
	{
		BOOK = 21 + 9 * TILEX,
		DOORTOBOSS = 21 + 6 * TILEX,
		DOORTOGARDEN = 21 + 13 * TILEX,
		NEKO = 20 + 11 * TILEX
	};

	int _count_cat;
	int _frame_cat;

public:
	gardenToBoss_5f() {};
	~gardenToBoss_5f() {};

	HRESULT init(CHRDIRECTION _chrdirection = CHRDIREC_UP, LOCATION _location = LOCATION_DEFAULT);
	void release();
	void update();
	void render();
	void Collision();
	void load(LOCATION location = LOCATION_DEFAULT);

	void setTrigger();
};

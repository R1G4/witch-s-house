#pragma once
#include "fifthFloorStage.h"
class prison_5f_skul : public fifthFloorStage
{
private:
	enum TRIGGER
	{
		DOORTOPRISON = 16 + 8 * TILEX,
		SKUL_1 = 20 + 6 * TILEX,
		SKUL_2 = 20 + 10 * TILEX,
		SKUL_3 = 24 + 10 * TILEX,
		SKUL_4 = 24 + 6 * TILEX,
		LEVER = 22 + 4 * TILEX,
		SKUL_5 = 26 + 5 * TILEX,
	};

	TRIGGER _trigger;
public:
	prison_5f_skul() {};
	~prison_5f_skul() {};

	HRESULT init();
	void release();
	void update();
	void render();
	void Collision();
	void load();

	void setTrigger();
};


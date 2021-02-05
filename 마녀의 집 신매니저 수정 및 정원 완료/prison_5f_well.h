#pragma once
#include "fifthFloorStage.h"
class prison_5f_well : public fifthFloorStage
{
private:
	enum TRIGGER
	{
		DOORTOPRISON = 18 + 12 * TILEX,
		FLOG = 19 + 7 * TILEX,
		WATER = 18 + 7 * TILEX,
	};

	TRIGGER _trigger;
public:
	prison_5f_well() {};
	~prison_5f_well() {};

	HRESULT init();
	void release();
	void update();
	void render();
	void Collision();
	void load();

	void setTrigger();
};


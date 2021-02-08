#pragma once
#include "fifthFloorStage.h"
class prison_5f_well : public fifthFloorStage
{
private:
	enum FROGORDER
	{
		FIRST,
		SECOND,
		THIRD,
		FORTH
	};
	enum TRIGGER
	{
		DOORTOPRISON = 18 + 12 * TILEX,
		FLOG = 19 + 7 * TILEX,
		WATER = 18 + 7 * TILEX,
	};

	TRIGGER _trigger;
	FROGORDER _fo;
	bool _timeToDead;

	int rnd_x[300];
	int rnd_y[300];
	int _count_line;
public:
	prison_5f_well() {};
	~prison_5f_well() {};

	HRESULT init(CHRDIRECTION _chrdirection = CHRDIREC_UP, LOCATION _location = LOCATION_DEFAULT);
	void release();
	void update();
	void render();
	void Collision();
	void load(LOCATION location = LOCATION_DEFAULT);

	void setTrigger();

	void deadTime();
};


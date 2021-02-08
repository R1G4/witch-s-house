#pragma once
#include "firstFloorStage.h"

class stage1_1 : public firstFloorStage
{
public:
	stage1_1();
	~stage1_1();

	HRESULT init();
	void release();
	void update();
	void render();
	void load();
};


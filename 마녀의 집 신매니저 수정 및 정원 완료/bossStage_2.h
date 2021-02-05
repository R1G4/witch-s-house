#pragma once
#include "bossStage.h"

class bossStage_2 :
	public bossStage
{
private:
	vector<Trigger> _vTrigger;

public:
	bossStage_2();
	~bossStage_2();

	HRESULT init();
	void release();
	void update();
	void render();
	void load();
	void activeTrigger();
	void activeCorr();
};


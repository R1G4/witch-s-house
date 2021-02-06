#pragma once
#include "bossStage.h"
class bossStage_5 :
	public bossStage
{
private:
	vector<Trigger> _vTrigger;

public:
	bossStage_5();
	~bossStage_5();

	HRESULT init();
	void release();
	void update();
	void render();
	void load();
	void activeTrigger();
	void activeCorr();

};


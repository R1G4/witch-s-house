#pragma once
#include "bossStage.h"
class bossStage_4 :
	public bossStage
{
private:
	vector<Trigger> _vTrigger;

public:
	bossStage_4();
	~bossStage_4();
	
	HRESULT init();
	void release();
	void update();
	void render();
	void load();
	void activeTrigger();
	void activeCorr();
};


#pragma once
#include "bossStage.h"
class bossStage_3 :
	public bossStage
{
private:
	vector<Trigger> _vTrigger;

public:
	bossStage_3();
	~bossStage_3();

	HRESULT init();
	void release();
	void update();
	void render();
	void load();
	void activeTrigger();
	void activeCorr();

};


#pragma once
#include "bossStage.h"

class bossStage_1 :
	public bossStage
{
private:
	vector<Trigger> _vTrigger;

public:
	bossStage_1();
	~bossStage_1();

	HRESULT init();
	virtual HRESULT init(int x, int y, CHRDIRECTION direc);
	void release();
	void update();
	void render();
	void load();
	void activeTrigger();
	void activeCorr();
};


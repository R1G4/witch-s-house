#pragma once
#include "gameNode.h"
#include "tank.h"
#include "enemyTank.h"
#include "tankMap.h"

class tankScene : public gameNode
{
private:
	tankMap* _tankMap;
	tank* _tank;
	enemyTank* _emTank;

public:
	tankScene();
	~tankScene();

	HRESULT init();
	void release();
	void update();
	void render();
};


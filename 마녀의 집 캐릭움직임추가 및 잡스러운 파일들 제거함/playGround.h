#pragma once
#include "gameNode.h"
#include "mapTool.h"
#include "garDen.h"
#include "garDenUnder.h"
#include "stage1_1.h"
#include "menu.h"
#include "levelChoiceScene.h"
#include "bossStage.h"
#include "bossStage_2.h"
#include "Player.h"
#include "castlefront.h"

class playGround : public gameNode
{
private:
	float alpha;
	int _currentFrameX;
	int _currentFrameY;
	int count;
	POINT pt;
	Image* viola;
	POINT camera;
	Player* _player;
	bossStage* _bossStage;
	bossStage_2* _bossStage_2;
public:
	playGround();
	~playGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
	void addAutoImage();
	void addScene();
	void addFrameImg();
	void addressLink();

};


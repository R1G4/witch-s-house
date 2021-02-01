#pragma once
#include "gameNode.h"
#include "mapTool.h"
#include "garDen.h"
#include "stage1_1.h"
#include "menu.h"
#include "levelChoiceScene.h"
#include "sound.h"
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
	sound* _sound;
public:
	playGround();
	~playGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
	void addBackGroundImage();
	void addScene();
};


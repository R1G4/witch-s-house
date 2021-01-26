#pragma once
#include "gameNode.h"
#include "mapTool.h"
#include "menu.h"
#include "levelChoiceScene.h"
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
public:
	playGround();
	~playGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
	void addScene();
};


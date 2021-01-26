#pragma once
#include "gameNode.h"
#include "menu.h"
#include "levelChoiceScene.h"

class playGround : public gameNode
{
public:
	playGround();
	~playGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	void addScene();
	virtual void render();
};


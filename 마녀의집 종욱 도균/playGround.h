#pragma once
#include "gameNode.h"
#include "mapTool.h"
#include "menu.h"
#include "continueScene.h"
#include "levelChoiceScene.h"

class playGround : public gameNode
{
private:
public:
	playGround();
	~playGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
	void addScene();
	void addImg();
};


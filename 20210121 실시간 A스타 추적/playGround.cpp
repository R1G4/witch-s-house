#include "stdafx.h"
#include "playGround.h"

playGround::playGround()
{
}


playGround::~playGround()
{
}

HRESULT playGround::init()
{
	gameNode::init(true);

	SCENEMANAGER->addScene("¶¥²ô¾À", new tankScene);
	SCENEMANAGER->changeScene("¶¥²ô¾À");

	
	return S_OK;
}


void playGround::release()
{
	gameNode::release();

	
}

void playGround::update()
{
	gameNode::update();

	SCENEMANAGER->update();
	
}


void playGround::render()
{
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//================ À§¿¡ °ÇµéÁö ¸¶¶ó ==============================
	
	SCENEMANAGER->render();

	//TIMEMANAGER->render(getMemDC());
	//================= ¾Æ·¡µµ °ÇµéÁö ¸¶¶ó ==============================
	_backBuffer->render(getHDC());
}


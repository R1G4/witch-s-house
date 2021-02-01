#include "stdafx.h"
#include "DASH.h"

void DASH::enterState()
{
	_player->setChrImg(FRAMEINFOMANAGER->FindImage("violaDash"));
	_player->setSpeed(8);
}

void DASH::updateState()
{
	_player->framePlay();
	_player->move();
	if (KEYMANAGER->isOnceKeyUp(VK_RIGHT) || KEYMANAGER->isOnceKeyUp(VK_LEFT) || KEYMANAGER->isOnceKeyUp(VK_UP) || KEYMANAGER->isOnceKeyUp(VK_DOWN))
	{
		_player->setState(CHR_IDLE);
	}
}

void DASH::exitState()
{
	_player->setFrameX(0);
}

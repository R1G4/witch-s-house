#include "stdafx.h"
#include "WALK.h"

void WALK::enterState()
{
	_player->setChrImg(FRAMEINFOMANAGER->FindImage("violaWalk"));
	_player->setSpeed(4);
}

void WALK::updateState()
{
	_player->framePlay();
	_player->move();
	if (KEYMANAGER->isOnceKeyUp(VK_RIGHT)||KEYMANAGER->isOnceKeyUp(VK_LEFT)||KEYMANAGER->isOnceKeyUp(VK_UP)||KEYMANAGER->isOnceKeyUp(VK_DOWN))
	{
		_player->setState(CHR_IDLE);
	}
}

void WALK::exitState()
{
	_player->setFrameX(0);
}

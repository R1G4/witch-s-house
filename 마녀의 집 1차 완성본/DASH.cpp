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

	if (_player->getPdirec() == CHRDIREC_LEFT)
	{
		if (KEYMANAGER->isOnceKeyUp(VK_LEFT))
			_player->setState(CHR_IDLE);
	}
	if (_player->getPdirec() == CHRDIREC_RIGHT)
	{
		if (KEYMANAGER->isOnceKeyUp(VK_RIGHT))
			_player->setState(CHR_IDLE);
	}
	if (_player->getPdirec() == CHRDIREC_UP)
	{
		if (KEYMANAGER->isOnceKeyUp(VK_UP))
			_player->setState(CHR_IDLE);
	}
	if (_player->getPdirec() == CHRDIREC_DOWN)
	{
		if (KEYMANAGER->isOnceKeyUp(VK_DOWN))
			_player->setState(CHR_IDLE);
	}



	/*
	if (KEYMANAGER->isOnceKeyUp(VK_RIGHT) || KEYMANAGER->isOnceKeyUp(VK_LEFT) || KEYMANAGER->isOnceKeyUp(VK_UP) || KEYMANAGER->isOnceKeyUp(VK_DOWN))
	{
		_player->setState(CHR_IDLE);
	}
	*/
}

void DASH::exitState()
{
	_player->setFrameX(0);
}

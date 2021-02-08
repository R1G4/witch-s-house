#include "stdafx.h"
#include "IDLE.h"
#include "Player.h"
void IDLE::enterState()
{
	_player->setChrImg(FRAMEINFOMANAGER->FindImage("violaIdle"));
}

void IDLE::updateState()
{
	_player->framePlay();
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
	{
		_player->setSearchRc(RectMakePivot(Vector2(_player->getPlayerLocX() - TILESIZE, _player->getPlayerLocY()), Vector2(30, 30), Pivot::LeftTop));

		_player->setDirec(CHRDIREC_LEFT);
		if(_player->getIsDash())_player->setState(CHR_DASH);
		else _player->setState(CHR_WALK);
	}
	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
	{
		_player->setSearchRc(RectMakePivot(Vector2(_player->getPlayerLocX() + TILESIZE, _player->getPlayerLocY()), Vector2(30, 30), Pivot::LeftTop));

		_player->setDirec(CHRDIREC_RIGHT);
		if (_player->getIsDash())_player->setState(CHR_DASH);
		else _player->setState(CHR_WALK);
	}
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		_player->setSearchRc(RectMakePivot(Vector2(_player->getPlayerLocX(), _player->getPlayerLocY() + TILESIZE), Vector2(30, 30), Pivot::LeftTop));

		_player->setDirec(CHRDIREC_DOWN);
		if (_player->getIsDash())_player->setState(CHR_DASH);
		else _player->setState(CHR_WALK);
	}
	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		_player->setSearchRc(RectMakePivot(Vector2(_player->getPlayerLocX(), _player->getPlayerLocY() - TILESIZE), Vector2(30, 30), Pivot::LeftTop));

		_player->setDirec(CHRDIREC_UP);
		if (_player->getIsDash())_player->setState(CHR_DASH);
		else _player->setState(CHR_WALK);
	}
	if (KEYMANAGER->isStayKeyDown(VK_SHIFT))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			_player->setDirec(CHRDIREC_LEFT);
			if (_player->getIsDash())_player->setState(CHR_WALK);
			else _player->setState(CHR_DASH);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			_player->setDirec(CHRDIREC_RIGHT);
			if (_player->getIsDash())_player->setState(CHR_WALK);
			else _player->setState(CHR_DASH);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			_player->setDirec(CHRDIREC_DOWN);
			if (_player->getIsDash())_player->setState(CHR_WALK);
			else _player->setState(CHR_DASH);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			_player->setDirec(CHRDIREC_UP);
			if (_player->getIsDash())_player->setState(CHR_WALK);
			else _player->setState(CHR_DASH);
		}
	}
}

void IDLE::exitState()
{
	_player->setFrameX(0);
}

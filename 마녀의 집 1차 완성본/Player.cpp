#include "stdafx.h"
#include "Player.h"
#include "IDLE.h"
#include "DASH.h"
#include "WALK.h"
HRESULT Player::init()
{
	_player.x = _startPosition.x*TILESIZE;
	_player.y = _startPosition.y*TILESIZE;

	_player.speed = 5.0f;
	setState(CHR_IDLE);
	_player.isDash = true;
	_player.alpha = 1;
	frameSpeed = 5;
	_player.rc = RectMakePivot(Vector2(_player.x+3, _player.y + 3), Vector2(TILESIZE/2, TILESIZE/2), Pivot::LeftTop);

	_open = false;
	return S_OK;
}

void Player::release()
{
}

void Player::update()
{
	
	//2월5일 UI합치면서 open 만들어둠
	if (_open)
	{
		if (KEYMANAGER->isOnceKeyDown('X')) _open = false;
	}
	
	if (!_open)
	{
		if (KEYMANAGER->isOnceKeyDown('X')) _open = true;
		chr_State->updateState();
		_player.frameY = (int)_player.direc;
		_player.rc = RectMakePivot(Vector2(_player.x + 3, _player.y + 3), Vector2(TILESIZE / 2, TILESIZE / 2), Pivot::LeftTop);
		framePlay();
	}
	
}

void Player::render()
{
	_player.img->SetAlpha(_player.alpha);
	CAMERAMANAGER->FrameRender(_player.img, Vector2(_player.x + TILESIZE/3,_player.rc.bottom-_player.img->GetFrameSize().y/2), _player.frameX, _player.frameY);
	if (KEYMANAGER->isToggleKey(VK_TAB))
	{
		CAMERAMANAGER->renderRc(_player.searchRc, D2D1::ColorF::Magenta, 1, 1);
		CAMERAMANAGER->renderRc(_player.rc, D2D1::ColorF::Blue, 1, 1);
	}
}

void Player::setState(STATE st)
{
	if (chr_State = NULL)chr_State->exitState();
	switch (st)
	{
	case CHR_IDLE:
		chr_State = new IDLE;
		break;
	case CHR_WALK:
		chr_State = new WALK;
		break;
	case CHR_DASH:
		chr_State = new DASH;
		break;
	case CHR_DEAD:
		break;
	}
	chr_State->LinkPlayerAddress(this);
	chr_State->enterState();
}

void Player::framePlay()
{
	_count++;
	if (_count %frameSpeed==0)
	{
		_player.frameX++;
		if (_player.frameX > _player.img->GetMaxFrameX()-1)_player.frameX = 0;
	}
}

void Player::move()
{
	switch (_player.direc)
	{
	case CHRDIREC_DOWN:
		_player.searchRc = RectMakePivot(Vector2(_player.x, _player.y + TILESIZE), Vector2(30, 30), Pivot::LeftTop);
		_player.y += _player.speed;

		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
			_player.x -= _player.speed;
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
			_player.x += _player.speed;
		break;
	case CHRDIREC_LEFT:
		_player.searchRc = RectMakePivot(Vector2(_player.x - TILESIZE, _player.y ), Vector2(30, 30), Pivot::LeftTop);
		_player.x -= _player.speed;

		if (KEYMANAGER->isStayKeyDown(VK_UP))
			_player.y -= _player.speed;
		if (KEYMANAGER->isStayKeyDown(VK_DOWN))
			_player.y += _player.speed;
		break;
	case CHRDIREC_RIGHT:
		_player.searchRc = RectMakePivot(Vector2(_player.x + TILESIZE, _player.y ), Vector2(30, 30), Pivot::LeftTop);
		_player.x += _player.speed;

		if (KEYMANAGER->isStayKeyDown(VK_UP))
			_player.y -= _player.speed;
		if (KEYMANAGER->isStayKeyDown(VK_DOWN))
			_player.y += _player.speed;
		break;
	case CHRDIREC_UP:
		_player.searchRc = RectMakePivot(Vector2(_player.x, _player.y - TILESIZE), Vector2(30, 30), Pivot::LeftTop);
		_player.y -= _player.speed;

		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
			_player.x -= _player.speed;
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
			_player.x += _player.speed;
		break;
	}
}

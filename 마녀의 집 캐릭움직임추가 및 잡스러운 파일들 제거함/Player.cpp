#include "stdafx.h"
#include "Player.h"
#include "IDLE.h"
#include "DASH.h"
#include "WALK.h"
HRESULT Player::init()
{
	cout << _startPosition.x << " " << _startPosition.y << endl;
	_player.x = _startPosition.x*TILESIZE;
	_player.y = _startPosition.y*TILESIZE;
	cout << _player.x << " " << _player.y << endl;
	_player.speed = 5.0f;
	setState(CHR_IDLE);
	_player.isDash = true;
	return S_OK;
}

void Player::release()
{
}

void Player::update()
{

	chr_State->updateState();
	_player.frameY = (int)_player.direc;
	_player.rc = RectMakePivot(Vector2(_player.x, _player.y), Vector2(TILESIZE-15, TILESIZE-15), Pivot::LeftTop);
	framePlay();
}

void Player::render()
{
	CAMERAMANAGER->FrameRender(_player.img, Vector2(_player.x + TILESIZE/3,_player.rc.bottom-_player.img->GetFrameSize().y/2), _player.frameX, _player.frameY);
	if (KEYMANAGER->isToggleKey(VK_TAB))CAMERAMANAGER->renderRc(_player.rc, D2D1::ColorF::Blue,1,1);
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
	if (_count %5==0)
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
		_player.y += _player.speed;
		break;
	case CHRDIREC_LEFT:
		_player.x -= _player.speed;
		break;
	case CHRDIREC_RIGHT:
		_player.x += _player.speed;
		break;
	case CHRDIREC_UP:
		_player.y -= _player.speed;
		break;
	}
}

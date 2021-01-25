#include "stdafx.h"
#include "jump.h"

HRESULT jump::init()
{
	_isJumping = _jumpPower = _gravity = 0;

	return S_OK;
}

void jump::release()
{
}

void jump::update()
{
	if (!_isJumping) return;

	*_y -= _jumpPower;
	_jumpPower -= _gravity;

	//현재 위치가 처음 뛴 위치보다 커진다면 (더 내려가려고 하면)
	if (_startY <= *_y)
	{
		_isJumping = false;
		*_y = _startY;
	}

}

void jump::render()
{
}

void jump::jumping(float* x, float* y, float power, float gravity)
{
	if (_isJumping) return;

	_isJumping = true;

	_x = x;
	_y = y;

	_startX = *x;
	_startY = *y;

	_gravity = gravity;
	_jumpPower = power;

}

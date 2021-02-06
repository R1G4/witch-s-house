#include "stdafx.h"
#include "bear.h"

HRESULT bear::init()
{
	return S_OK;
}

HRESULT bear::init(float x, float y)
{
	_image = IMAGEMANAGER->AddFrameImage("°õ", L"Image/tempFrameImg/°õ.png", 1, 4);
	_x = x * TILESIZE;
	_y = y * TILESIZE;
	//_rc = RectMakePivot(Vector2(_x+TILESIZE/2, _y+TILESIZE/2), Vector2(TILESIZE, TILESIZE), Pivot::LeftTop);
	_speed = 10;	// ¹Ì¼¼ Á¶Á¤À» ÇÕ½Ã´Ù
	_currentFrameX = _currentFrameY = 0;

	return S_OK;
}

void bear::update()
{
	count++;
	if (count % 9 == 0)
	{
		if (_currentFrameY >= 3) _currentFrameY = -1;
		_currentFrameX = 0;

		_currentFrameY++;
		count = 0;
	}

	_rc = RectMakePivot(Vector2(_x + TILESIZE / 2, _y + TILESIZE / 2), Vector2(TILESIZE, TILESIZE), Pivot::Center);
}

void bear::release()
{
}

void bear::render()
{
	CAMERAMANAGER->FrameRender(_image, Vector2(_x + TILESIZE / 2, _y - 40), _currentFrameX, _currentFrameY);
	if (KEYMANAGER->isToggleKey(VK_TAB))
	{
		CAMERAMANAGER->renderRc(_rc, D2D1::ColorF::White, 1, 3);
	}
}

#include "stdafx.h"
#include "boss.h"
#include "garDen.h"

HRESULT boss::init()
{
	return S_OK;
}

HRESULT boss::init(float x, float y)
{
	_image = IMAGEMANAGER->AddFrameImage("보스", L"Image/tempFrameImg/boss.png", 8, 4);
	_x = x * TILESIZE;
	_y = y * TILESIZE;
	//_rc = RectMakePivot(Vector2(_x+TILESIZE/2, _y+TILESIZE/2), Vector2(TILESIZE, TILESIZE), Pivot::LeftTop);
	_speed = 10;	// 미세 조정을 합시다
	_currentFrameX = _currentFrameY = 0;
	_direction = BOSSDIRECTION_LEFT;

	return S_OK;
}

void boss::update()
{
	count++;
	if (count % 5 == 0)
	{
		switch (_direction)
		{
		case BOSSDIRECTION_LEFT:
			if (_currentFrameX >= 7) _currentFrameX = -1;
			_currentFrameY = 1;
			_x = _x - _speed;
			_currentFrameX++;
			break;
		case BOSSDIRECTION_RIGHT:
			if (_currentFrameX >= 7) _currentFrameX = -1;
			_currentFrameY = 2;
			_x = _x + _speed;
			_currentFrameX++;
			break;
		case BOSSDIRECTION_UP:
			if (_currentFrameX >= 7) _currentFrameX = -1;
			_currentFrameY = 3;
			_y = _y - _speed;
			_currentFrameX++;
			break;
		case BOSSDIRECTION_DOWN:
			if (_currentFrameX >= 7) _currentFrameX = -1;
			_currentFrameY = 0;
			_y = _y + _speed;
			_currentFrameX++;
			break;
		default:
			break;
		}
		count = 0;
	}

	FloatRect previous_rc;	// 이전 타일과 현재 타일 위치를 확인해 위치 변환을 위한 변수 
	previous_rc = _rc;

	_rc = RectMakePivot(Vector2(_x + TILESIZE / 2, _y + TILESIZE / 2), Vector2(TILESIZE, TILESIZE), Pivot::Center);
	_sub_rc = RectMakePivot(Vector2(_x + TILESIZE / 2, _y + TILESIZE / 2), Vector2(1, 1), Pivot::Center);

	if (previous_rc.top > _sub_rc.top)
		_direction = BOSSDIRECTION_UP;
	if (previous_rc.bottom < _sub_rc.bottom)
		_direction = BOSSDIRECTION_DOWN;
	if (previous_rc.left > _sub_rc.left)
		_direction = BOSSDIRECTION_LEFT;
	if (previous_rc.right < _sub_rc.right)
		_direction = BOSSDIRECTION_RIGHT;
}

void boss::release()
{
}

void boss::render()
{
	CAMERAMANAGER->FrameRender(_image, Vector2(_x + TILESIZE / 2, _y), _currentFrameX, _currentFrameY);
	if (KEYMANAGER->isToggleKey(VK_TAB))
	{
		CAMERAMANAGER->renderRc(_rc,D2D1::ColorF::White, 1, 3);
	}
}

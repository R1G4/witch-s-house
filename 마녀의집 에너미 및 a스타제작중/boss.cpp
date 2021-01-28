#include "stdafx.h"
#include "boss.h"

HRESULT boss::init()
{
	return S_OK;
}

HRESULT boss::init(float x, float y)
{
	_image = IMAGEMANAGER->AddFrameImage("보스", L"Image/tempFrameImg/boss.png", 8, 4);
	_x = x * TILESIZE;
	_y = y * TILESIZE;
	_rc = RectMakePivot(Vector2(_x, _y), Vector2(TILESIZE, TILESIZE), Pivot::LeftTop);
	_speed = 2;	// 미세 조정을 합시다
	_currentFrameX = _currentFrameY = 0;
	_direction = BOSSDIRECTION_LEFT;

	return S_OK;
}

void boss::update(POINT camera)
{
	count++;
	if (count % 5 == 0)
	{
		switch (_direction)
		{
		case BOSSDIRECTION_LEFT:
			if (_currentFrameX >= 7) _currentFrameX = 0;
			_currentFrameY = 1;
			_x = _x - _speed;
			_currentFrameX++;
			break;
		case BOSSDIRECTION_RIGHT:
			if (_currentFrameX >= 7) _currentFrameX = 0;
			_currentFrameY = 2;
			_x = _x + _speed;
			_currentFrameX++;
			break;
		case BOSSDIRECTION_UP:
			if (_currentFrameX >= 7) _currentFrameX = 0;
			_currentFrameY = 3;
			_y = _y - _speed;
			_currentFrameX++;
			break;
		case BOSSDIRECTION_DOWN:
			if (_currentFrameX >= 7) _currentFrameX = 0;
			_currentFrameY = 0;
			_y = _y + _speed;
			_currentFrameX++;
			break;
		default:
			break;
		}
		count = 0;
	}

	_rc = RectMakePivot(Vector2(_x - camera.x, _y - camera.y), Vector2(TILESIZE, TILESIZE), Pivot::LeftTop);

	if (Vector2InRect(&_rc, &Vector2(_ptMouse)))
	{
		cout << "들어왓는감" << endl;
	}
}

void boss::release()
{
}

void boss::render(POINT camera)
{
	_image->FrameRender(Vector2(_x - camera.x, _y - camera.y), _currentFrameX, _currentFrameY);

	if (KEYMANAGER->isToggleKey(VK_TAB))
	{
		
	}
}

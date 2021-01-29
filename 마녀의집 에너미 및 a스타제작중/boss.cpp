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
	//_rc = RectMakePivot(Vector2(_x+TILESIZE/2, _y+TILESIZE/2), Vector2(TILESIZE, TILESIZE), Pivot::LeftTop);
	_speed = 2;	// 미세 조정을 합시다
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
//	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
//	{
//		
//		_x -=5;
//		//_rc.Move(Vector2(-4.0, 0));
////		cout << _x / TILESIZE << " " << _y / TILESIZE << " " << _rc.left << endl;
//	}
//	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
//	{
//		_x +=5;
//		//_rc.Move(Vector2(4.0, 0));
////		cout << _x/TILESIZE<<" "<<_y/TILESIZE << " " << _rc.left << endl;
//	}

	_rc = RectMakePivot(Vector2(_x+TILESIZE/2 , _y+TILESIZE/2 ), Vector2(TILESIZE, TILESIZE), Pivot::Center);
	//_rc.Update(Vector2(_x+TILESIZE/2-CAMERAMANAGER->getCameraX(), _y+TILESIZE/2-CAMERAMANAGER->getCameraY()), Vector2(TILESIZE, TILESIZE), Pivot::Center);
//	cout << _x / TILESIZE << " " << _y / TILESIZE << " " << _rc.left << endl;

	if (Vector2InRect(&_rc, &CAMERAMANAGER->getWorldMouse()))
	{
		cout << "들어왓는감" << endl;
	}
}

void boss::release()
{
}

void boss::render()
{
	//_image->FrameRender(Vector2(_rc.left, _rc.top), _currentFrameX, _currentFrameY);
	//CAMERAMANAGER->FrameRender(_image,Vector2(_rc.left, _rc.top), _currentFrameX, _currentFrameY);
	CAMERAMANAGER->FrameRender(_image, Vector2(_x, _y), _currentFrameX, _currentFrameY);
	if (KEYMANAGER->isToggleKey(VK_TAB))
	{
		CAMERAMANAGER->renderRc(_rc,D2D1::ColorF::White, 1, 3);
		//_D2DRenderer->DrawRectangle(_rc);
		//cout << _rc.left << " " << _rc.right << " " << _rc.top << " " << _rc.bottom<<" "<<_x<<endl ;
	}
}

#include "stdafx.h"
#include "player.h"

player::player()
{
}

player::~player()
{
}

HRESULT player::init()
{
	ImageManager::GetInstance()->AddFrameImage("violaIdle", L"Image/player/violaIdle.png", 16, 4);
	viola = ImageManager::GetInstance()->FindImage("viloaIdle");
	_currentFrameX = 0;
	_currentFrameY = 0;
	/*pt.x = WINSIZEX / 2;
	pt.y = WINSIZEY / 2;*/
	pt.x = 100;
	pt.y = 100;

	
	
	return S_OK;
}

void player::release()
{
}

void player::update()
{
	gameNode::update();
	if (KEYMANAGER->isStayKeyDown(VK_UP))_alpha += 0.01;
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))_alpha -= 0.01;
	if (KEYMANAGER->isOnceKeyUp(VK_RIGHT) || KEYMANAGER->isOnceKeyUp(VK_LEFT)
		|| KEYMANAGER->isOnceKeyUp(VK_UP) || KEYMANAGER->isOnceKeyUp(VK_DOWN))_currentFrameX = 0;

	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		pt.x += 3;
		_currentFrameY = 2;

		_count++;
		if (_count % 4 == 0)
		{
			_currentFrameX++;
			if (_currentFrameX > ImageManager::GetInstance()->FindImage("violaIdle")->GetMaxFrameX() - 1)_currentFrameX = 0;
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		pt.x -= 3;
		_currentFrameY = 1;
		_count++;
		if (_count % 4 == 0)
		{
			_currentFrameX++;
			if (_currentFrameX > ImageManager::GetInstance()->FindImage("violaIdle")->GetMaxFrameX() - 1)_currentFrameX = 0;
		}
	}

	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		pt.y -= 3;
		_currentFrameY = 3;
		_count++;
		if (_count % 4 == 0)
		{
			_currentFrameX++;
			if (_currentFrameX > ImageManager::GetInstance()->FindImage("violaIdle")->GetMaxFrameX() - 1)_currentFrameX = 0;
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		pt.y += 3;
		_currentFrameY = 0;
		_count++;
		if (_count % 4 == 0)
		{
			_currentFrameX++;
			if (_currentFrameX > ImageManager::GetInstance()->FindImage("violaIdle")->GetMaxFrameX() - 1)_currentFrameX = 0;
		}
	}

}

void player::render()
{
	Image* frameImage2 = ImageManager::GetInstance()->FindImage("violaIdle");
	frameImage2->SetScale(1);
	frameImage2->FrameRender(Vector2(pt.x, pt.y), _currentFrameX, _currentFrameY);
}

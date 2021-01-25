#include "stdafx.h"
#include "tank.h"


tank::tank()
{
}


tank::~tank()
{
}

HRESULT tank::init()
{
	_direction = TANKDIRECTION_RIGHT;
	
	_image = new image;
	_image->init("tank.bmp", 0, 0, 256, 128, 8, 4, true, RGB(255, 0, 255));
	//_image = IMAGEMANAGER->addFrameImage("tank", "tank.bmp", 0, 0, 256, 128, 8, 4, true, RGB(255, 0, 255));
	_speed = 100.0f;

	_tileX = _tileY = 0;

	return S_OK;
}

void tank::release()
{
}

void tank::update()
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		_direction = TANKDIRECTION_LEFT;
		tankMove();
	}

	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		_direction = TANKDIRECTION_RIGHT;
		tankMove();
	}

	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		_direction = TANKDIRECTION_UP;
		tankMove();
	}

	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		_direction = TANKDIRECTION_DOWN;
		tankMove();
	}
}

void tank::render()
{
	_image->frameRender(getMemDC(), _rc.left, _rc.top);
}

void tank::tankMove()
{
	RECT rcCollision;	//임의의 충돌판정용 렉트
	int tileIndex[2];	//이동방향에 따라 타일속성 검출계산용(몇 바이 몇에 있다)
	int tileX, tileY;	//실제 탱크가 어느 타일에 위치하는지 계산용(left, top 기준일때)
	
	

	float elapsedTime = TIMEMANAGER->getElapsedTime();
	float moveSpeed = elapsedTime * _speed * 2;

	//탱크 렉트를 충돌용 렉트
	rcCollision = _rc;

	switch (_direction)
	{
	case TANKDIRECTION_LEFT:
		_image->setFrameX(0);
		_image->setFrameY(3);

		_x -= moveSpeed;

		rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());

		break;
	case TANKDIRECTION_UP:
		_image->setFrameX(0);
		_image->setFrameY(0);

		_y -= moveSpeed;

		rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
		break;
	case TANKDIRECTION_RIGHT:
		_image->setFrameX(0);
		_image->setFrameY(2);

		_x += moveSpeed;

		rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
		break;
	case TANKDIRECTION_DOWN:
		_image->setFrameX(0);
		_image->setFrameY(1);

		_y += moveSpeed;

		rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
		break;
	}

	//3.여러분이 조금있다 과제를 하실땐 충돌판정용 렉트를 살짝 깍아주어야 합니다.
	rcCollision.left += 2;
	rcCollision.top += 2;
	rcCollision.right -= 2;
	rcCollision.bottom -= 2;

	//4. 내 탱크좌표가 몇 번째 타일 인덱스에 있는지 계산을 해주세요
	tileX = rcCollision.left / TILESIZE;
	tileY = rcCollision.top / TILESIZE;

	//5.여길 계산하시오 (하일라이뜨)
	switch (_direction)
	{
	case TANKDIRECTION_LEFT:
		tileIndex[0] = tileX + tileY * TILEX;
		tileIndex[1] = tileX + (tileY + 1) * TILEX;
		break;
	case TANKDIRECTION_UP:
		tileIndex[0] = tileX + tileY * TILEX;
		tileIndex[1] = (tileX + 1) + tileY * TILEX;
		break;
	case TANKDIRECTION_RIGHT:
		tileIndex[0] = (tileX + tileY * TILEX) + 1;
		tileIndex[1] = (tileX + (tileY + 1) * TILEX) + 1;
		break;
	case TANKDIRECTION_DOWN:
		tileIndex[0] = (tileX + tileY * TILEX) + TILEX;
		tileIndex[1] = (tileX + 1 + tileY * TILEX) + TILEY;
		break;

	}

	//6. 예외처리도 해주세요
	//못가는 타일과 부딪혔다! 그럼 막아랏! 
	for (int i = 0; i < 2; i++)
	{
		RECT temp;
		if (((_tankMap->getAttribute()[tileIndex[i]] & ATTR_UNMOVE) == ATTR_UNMOVE) &&
			IntersectRect(&temp, &_tankMap->getTile()[tileIndex[i]].rc, &rcCollision))
		{
			switch (_direction)
			{
				case TANKDIRECTION_LEFT:
					_rc.left = _tankMap->getTile()[tileIndex[i]].rc.right;
					_rc.right = _rc.left + 28;

					_x = (_rc.left + _rc.right) / 2;
				break;
				case TANKDIRECTION_UP:
					_rc.top = _tankMap->getTile()[tileIndex[i]].rc.bottom;
					_rc.bottom = _rc.top + 28;

					_y = (_rc.top + _rc.bottom) / 2;
				break;
				case TANKDIRECTION_RIGHT:
					_rc.right = _tankMap->getTile()[tileIndex[i]].rc.left;
					_rc.left = _rc.right - 28;

					_x = (_rc.left + _rc.right) / 2;
				break;
				case TANKDIRECTION_DOWN:
					_rc.bottom = _tankMap->getTile()[tileIndex[i]].rc.top;
					_rc.top = _rc.bottom - 28;

					_y = (_rc.top + _rc.bottom) / 2;
				break;
			}
		}
	}

	_tileX = tileX;
	_tileY = tileY;

	rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(),
		_image->getFrameHeight());

	_rc = rcCollision;
}

void tank::setTankPosition()
{
	_rc = _tankMap->getTile()[_tankMap->getPosFirst()].rc;

	_x = (_rc.left + _rc.right) / 2;
	_y = (_rc.top + _rc.bottom) / 2;
}

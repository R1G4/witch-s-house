#include "stdafx.h"
#include "astarTile.h"

astarTile::astarTile()
	:_totalCost(0), _costToGoal(0), _costFromStart(0), _parentNode(nullptr), _idX(0), _idY(0), _number(0), _color(D2D1::ColorF::White)
{
}

astarTile::~astarTile()
{
}

HRESULT astarTile::init(int idX, int idY)
{
	_center = PointMake(idX * TILESIZE + (TILESIZE / 2),
		idY * TILESIZE + (TILESIZE / 2));

	_idX = idX;
	_idY = idY;

	_rc = RectMakeCenter(_center.x, _center.y, TILESIZE, TILESIZE);


	return S_OK;
}

void astarTile::release()
{
}

void astarTile::update()
{
}

void astarTile::render()
{
	FloatRect f_rc;
	f_rc = FloatRect(_rc);
	CAMERAMANAGER->renderFillRc(f_rc, _color, 0.5f);
	CAMERAMANAGER->renderRc(FloatRect(_rc.left, _rc.top, _rc.right, _rc.bottom), D2D1::ColorF::Blue, 1.0f, 1.0f);
	/*FillRect(getMemDC(), &_rc, _brush);

	SelectObject(getMemDC(), (HPEN)_pen);
	RectangleMake(getMemDC(), _rc.left, _rc.top, _rc.right, _rc.bottom);

	if (_number > 0)
	{
		char str[128];
		ZeroMemory(str, 126);
		sprintf_s(str, "%d", _number);
		TextOut(getMemDC(), _rc.left, _rc.top, str, strlen(str));
	}*/
}

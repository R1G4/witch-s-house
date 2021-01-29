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
	if (_color == D2D1::ColorF::White)
		CAMERAMANAGER->renderFillRc(f_rc, _color, 0.0f);
	else
		CAMERAMANAGER->renderFillRc(f_rc, _color, 1.0f);

	CAMERAMANAGER->renderRc(FloatRect(_rc.left, _rc.top, _rc.right, _rc.bottom), D2D1::ColorF::Blue, 1.0f, 1.0f);
	/*FillRect(getMemDC(), &_rc, _brush);

	SelectObject(getMemDC(), (HPEN)_pen);
	RectangleMake(getMemDC(), _rc.left, _rc.top, _rc.right, _rc.bottom);
	*/
	if (_number > 0)
	{
		char str[128];
		string str_s;
		wstring str_w;
		ZeroMemory(str, 126);
		sprintf_s(str, "%d", _number);
		str_s = str;
		str_w.assign(str_s.begin(), str_s.end());
		//_D2DRenderer->RenderText(f_rc.left, f_rc.top, str_w, 10, D2DRenderer::DefaultBrush::White);
		CAMERAMANAGER->txtRender(str, Vector2(f_rc.left, f_rc.top), 10);
	}
}

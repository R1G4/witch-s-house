#include "stdafx.h"
#include "status.h"

status::status()
{
}

status::~status()
{
}

HRESULT status::init()
{
	_status = IMAGEMANAGER->AddImage("status", L"image/UI/status2.png");
	_menu = IMAGEMANAGER->AddImage("playerMenu", L"image/UI/bar7.png");
	_load = IMAGEMANAGER->AddImage("load", L"image/UI/bar99.png");
	_x = WINSIZEX / 2;
	_y = WINSIZEY / 2;
	_rcAlpha = 1.0f;
	_rcAlphaChange = 0.03f;
	_rc = RectMakePivot(Vector2(_x - 500, _y + 200), Vector2(120, 60), Pivot::Center);
	_rc1 = RectMakePivot(Vector2(_x - 500, _y + 140), Vector2(120, 30), Pivot::Center);
	_rc2 = RectMakePivot(Vector2(_x - 500, _y + 200), Vector2(120, 30), Pivot::Center);
	_rc3 = RectMakePivot(Vector2(_x - 500, _y + 260), Vector2(120, 30), Pivot::Center);
	_isMenu = false;
	_isChoice = false;
	_isLoad = false;
	return S_OK;
}

void status::release()
{
}

void status::update()
{
	rcAlphaChange();
	FloatRect _temp;
	_rc = RectMakePivot(Vector2(_x - 500, _y + 200), Vector2(120, 60), Pivot::Center);
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && _rc.bottom <= WINSIZEY / 2 + 280)
		_y += 60;
	if (KEYMANAGER->isOnceKeyDown(VK_UP) && _rc.top >= WINSIZEY / 2 + 170)
		_y -= 60;

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE) && !_isChoice)
	{
		cout << "bbb";
		_isChoice = true;
		_x = WINSIZEX / 2;
		_y = WINSIZEY / 2;
	}

	//아이템창
	if (IntersectRectToRect(&_temp, &_rc, &_rc1))
	{
		_rc = RectMakePivot(Vector2(_x - 510, _y + 210), Vector2(100, 40), Pivot::Center);
	}
	//불러오기
	if (IntersectRectToRect(&_temp, &_rc, &_rc2))
	{
		_rc = RectMakePivot(Vector2(_x - 500, _y + 200), Vector2(120, 60), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			cout << "qqq" << endl;
			//_isChoice = false;
		}
	}
	//설정
	if (IntersectRectToRect(&_temp, &_rc, &_rc3))
	{
		_rc = RectMakePivot(Vector2(_x - 530, _y + 185), Vector2(70, 40), Pivot::Center);
	}

}

void status::rcAlphaChange()
{
	if (_rcAlpha >= 0.9f)
		_rcAlphaChange = -0.03f;

	if (_rcAlpha <= 0.35f)
		_rcAlphaChange = 0.03f;

	_rcAlpha += _rcAlphaChange;
}

void status::render()
{
	//if (_isMenu)
	if (KEYMANAGER->isToggleKey('X'))
	{
		_isMenu = true;
		_isLoad = true;

		//D2DINS->GetInstance()->RenderText(80, WINSIZEY / 2 + 130, L"소지품", 30, D2DRenderer::DefaultBrush::White);
		//D2DINS->GetInstance()->RenderText(80, WINSIZEY / 2 + 180, L"불러오기", 30, D2DRenderer::DefaultBrush::White);
		//D2DINS->GetInstance()->RenderText(80, WINSIZEY / 2 + 230, L"설정", 30, D2DRenderer::DefaultBrush::White);

		_menu = IMAGEMANAGER->FindImage("playerMenu");
		_menu->Render(Vector2(170, WINSIZEY / 2 + 200));

		_status = IMAGEMANAGER->FindImage("status");
		_status->Render(Vector2(WINSIZEX / 2 + 130, WINSIZEY / 2 + 200));

		D2DINS->GetInstance()->RenderText(80, WINSIZEY / 2 + 130, L"소지품", RGB(255, 255, 255), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(80, WINSIZEY / 2 + 180, L"불러오기", RGB(255, 255, 255), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(80, WINSIZEY / 2 + 230, L"설정", RGB(255, 255, 255), 0.85f, 30);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
	}

	if (_isMenu && _isLoad)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			cout << "www" << endl;
			_openMenu = LOAD;
			//_load = IMAGEMANAGER->FindImage("load");
			//_load->Render(Vector2(WINSIZEX / 2 + 430, WINSIZEY / 2 + 200));
		}
	}

	if (_openMenu == LOAD)
	{
		//cout << "lll" << endl;
		_load = IMAGEMANAGER->FindImage("load");
		_load->Render(Vector2(650, 500));
	}
	/*else
	{
		_menu = IMAGEMANAGER->FindImage("playerMenu");
		_menu->Render(Vector2(0, 0));

		_status = IMAGEMANAGER->FindImage("status");
		_status->Render(Vector2(0,0));
	}*/
}

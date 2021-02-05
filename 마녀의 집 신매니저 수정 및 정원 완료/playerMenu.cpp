#include "stdafx.h"
#include "playerMenu.h"

playerMenu::playerMenu()
{
}

playerMenu::~playerMenu()
{
}

HRESULT playerMenu::init()
{
	_status = IMAGEMANAGER->AddImage("status", L"image/UI/status2.png");
	_menu = IMAGEMANAGER->AddImage("playerMenu", L"image/UI/bar7.png");

	_settings = new settings;
	_settings->init();
	_load = new load;
	_load->init();
	_item = new item;
	_item->init();

	_x = 160;
	_y = 505;
	_rcAlpha = 1.0f;
	_rcAlphaChange = 0.03f;
	_rc = RectMakePivot(Vector2(_x, _y), Vector2(180, 45), Pivot::Center);

	for (int i = 0; i < 3; i++)
	{
		_rcMenu[i] = RectMakePivot(Vector2(_x, _y - 20 + i * 70), Vector2(120, 50), Pivot::Center);
	}

	_isMenu = false;
	_isCheck = false;
	_isLoad = false;
	return S_OK;
}

void playerMenu::release()
{
}

void playerMenu::update()
{
	rcAlphaChange();
	FloatRect _temp;
	_rc = RectMakePivot(Vector2(_x, _y), Vector2(180, 45), Pivot::Center);

	//메뉴 열려있을때 아래랑 같은건데일단 킵
	//if (_isMenu)
	//{
	//	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	//	{
	//		if (_rc.bottom <= _rcMenu[2].top)
	//		{
	//			_y += 50;
	//			SOUNDMANAGER->play("cursor", 0.5f);
	//		}
	//	}
	//	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	//	{
	//		if (_rc.top >= _rcMenu[0].bottom)
	//		{
	//			_y -= 50;
	//			SOUNDMANAGER->play("cursor", 0.5f);
	//		}
	//	}
	//}

	//메뉴 열려있을때
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && _rc.bottom <= _rcMenu[2].top && _isMenu)
	{
		_y += 50;
		SOUNDMANAGER->play("cursor", 0.5f);
	}
	if (KEYMANAGER->isOnceKeyDown(VK_UP) && _rc.top >= _rcMenu[0].bottom && _isMenu)
	{
		_y -= 50;
		SOUNDMANAGER->play("cursor", 0.5f);
	}

	//if (KEYMANAGER->isOnceKeyDown(VK_SPACE) && !_isCheck)
	//{
	//	_isCheck = true;
	//	_x = WINSIZEX / 2;
	//	_y = WINSIZEY / 2;
	//}

	//아이템, 불러오기, 설정

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE) && _isMenu)
	{
		//아이템
		if (IntersectRectToRect(&_temp, &_rc, &_rcMenu[0]))
		{
			SOUNDMANAGER->play("cursor", 0.5f);
			_openMenu = ITEM;
		}
		//불러오기
		if (IntersectRectToRect(&_temp, &_rc, &_rcMenu[1]))
		{
			SOUNDMANAGER->play("cursor", 0.5f);
			_openMenu = LOAD;
		}
		//설정
		if (IntersectRectToRect(&_temp, &_rc, &_rcMenu[2]))
		{
			SOUNDMANAGER->play("cursor", 0.5f);
			_openMenu = SETTINGS;
		}
	}

	//아이템창 화면일때
	if (_openMenu == ITEM)
	{
		//아이템 클래스 만들고 넣어주기
	}

	//불러오기 화면일때
	if (_openMenu == LOAD)
	{
		_load->update();
	}
	//설정 화면일때
	if (_openMenu == SETTINGS)
	{
		_settings->update();
	}

	//메뉴로 돌아가기
	if (KEYMANAGER->isOnceKeyDown(VK_BACK) && (_openMenu == LOAD || _openMenu == ITEM || _openMenu == SETTINGS))
	{
		SOUNDMANAGER->play("click", 0.5f);
		_openMenu = MENU;
	}

}

void playerMenu::rcAlphaChange()
{
	if (_rcAlpha >= 0.9f)
		_rcAlphaChange = -0.03f;

	if (_rcAlpha <= 0.35f)
		_rcAlphaChange = 0.03f;

	_rcAlpha += _rcAlphaChange;
}

void playerMenu::render()
{
	if (/*_openMenu == MENU*/_openMenu != LOAD && _openMenu != ITEM && _openMenu != SETTINGS)
	{
		openMenu();
	}

	if (_openMenu == LOAD)
	{
		_load->render();
	}

	if (_openMenu == SETTINGS)
	{
		_settings->render();
	}

	if (_openMenu == ITEM)
	{
		_item->render();
	}
}

void playerMenu::openMenu()
{
	if (KEYMANAGER->isToggleKey('X'))
	{
		cout << "aaa" << endl;
		_isMenu = true;
		_isLoad = true;

		//메뉴 칸 렉트 테스트용
		//for (int i = 0; i < 3; i++)
		//{
		//	D2DINS->GetInstance()->DrawRectangle(_rcMenu[i], D2D1::ColorF::Yellow, _rcAlpha, 1.0f);
		//	D2DINS->FillRectangle(_rcMenu[i], D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		//}

		_menu = IMAGEMANAGER->FindImage("playerMenu");
		_menu->Render(Vector2(170, WINSIZEY / 2 + 200));

		_status = IMAGEMANAGER->FindImage("status");
		_status->Render(Vector2(WINSIZEX / 2 + 130, WINSIZEY / 2 + 200));
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 100, WINSIZEY / 2 + 160, L"비올라", RGB(255, 255, 255), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 100, WINSIZEY / 2 + 200, L"나이", RGB(0, 0, 204), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 50, WINSIZEY / 2 + 200, L"13", RGB(255, 255, 255), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 200, WINSIZEY / 2 + 200, L"체력", RGB(0, 0, 204), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 300, WINSIZEY / 2 + 200, L"10", RGB(255, 255, 255), 0.85f, 30);


		D2DINS->GetInstance()->RenderText(80, WINSIZEY / 2 + 130, L"소지품", RGB(255, 255, 255), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(80, WINSIZEY / 2 + 180, L"불러오기", RGB(255, 255, 255), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(80, WINSIZEY / 2 + 230, L"설정", RGB(255, 255, 255), 0.85f, 30);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);

		//if (_isMenu && _isLoad)
		//{
		//	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		//	{
		//		cout << "www" << endl;
		//		_openMenu = LOAD;
		//	}
		//}

	}
	else
	{
		_isMenu = false;
	}

}




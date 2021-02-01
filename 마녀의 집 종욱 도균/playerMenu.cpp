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
	_load = IMAGEMANAGER->AddImage("load", L"image/UI/load.png");
	_loadTop = IMAGEMANAGER->AddImage("loadTop", L"image/UI/loadTop.png");
	_item = IMAGEMANAGER->AddImage("item", L"image/UI/item2.png");
	_itemTop = IMAGEMANAGER->AddImage("itemTop", L"image/UI/itemTop2.png");

	_volume = 0.5f;
	_bgv = 0.5f;
	_percent = 50;
	_bgvPercent = 0;
	_x = 160;
	_y = 505;
	_x2 = 650;
	_y2 = 230;
	_x3 = 640;
	_y3 = WINSIZEY/2 + 20;
	_rcAlpha = 1.0f;
	_rcAlphaChange = 0.03f;
	_rc = RectMakePivot(Vector2(_x, _y), Vector2(180, 45), Pivot::Center);
	_rcLoadChoice = RectMakePivot(Vector2(_x2, _y2), Vector2(600, 80), Pivot::Center);
	_rcSettingChoice = RectMakePivot(Vector2(_x3, _y3), Vector2(400, 50), Pivot::Center);

	for (int i = 0; i < 3; i++)
	{
		_rcMenu[i] = RectMakePivot(Vector2(_x, _y-20 + i * 70), Vector2(120, 50), Pivot::Center);
	}

	for (int i = 0; i < 5; i++)
	{
		_rcLoad[i] = RectMakePivot(Vector2(650, 230 + i * 90), Vector2(600, 80), Pivot::Center);
	}
	
	for (int i = 0; i < 7; i++)
	{
		_rcSettings[i] = RectMakePivot(Vector2(630, WINSIZEY / 2 + 20 + i * 50), Vector2(400, 30), Pivot::Center);
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
	_rcLoadChoice = RectMakePivot(Vector2(_x2, _y2), Vector2(600, 80), Pivot::Center);
	_rcSettingChoice = RectMakePivot(Vector2(_x3, _y3), Vector2(400, 50), Pivot::Center);

	//메뉴 열려있을때 아래랑 같은건데일단 
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
	if (IntersectRectToRect(&_temp, &_rc, &_rcMenu[0]) && _isMenu)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			SOUNDMANAGER->play("cursor", 0.5f);
			_openMenu = ITEM;
		}
	}
	if (IntersectRectToRect(&_temp, &_rc, &_rcMenu[1]))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			SOUNDMANAGER->play("cursor", 0.5f);
			_openMenu = LOAD;
		}
	}
	if (IntersectRectToRect(&_temp, &_rc, &_rcMenu[2]))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			SOUNDMANAGER->play("cursor", 0.5f);
			_openMenu = SETTINGS;
		}
	}

	//불러오기 화면일때
	if (_openMenu == LOAD)
	{
		if (KEYMANAGER->isOnceKeyDown('S') && _rcLoadChoice.bottom <= _rcLoad[4].top)
		{
			SOUNDMANAGER->play("cursor", 0.5f);
			_y2 += 90;
		}
		if (KEYMANAGER->isOnceKeyDown('W') && _rcLoadChoice.top >= _rcLoad[0].bottom)
		{
			SOUNDMANAGER->play("cursor", 0.5f);
			_y2 -= 90;
		}
	
	}
	//설정 화면일때
	if (_openMenu == SETTINGS)
	{
		if (KEYMANAGER->isOnceKeyDown('S') && _rcSettingChoice.bottom <= _rcSettings[6].top)
		{
			SOUNDMANAGER->play("cursor", 0.5f);
			_y3 += 50;
		}
		if (KEYMANAGER->isOnceKeyDown('W') && _rcSettingChoice.top >= _rcSettings[0].bottom)
		{
			SOUNDMANAGER->play("cursor", 0.5f);
			_y3 -= 50;
		}

		//볼륨 문자열 조졀하려고
		mv = "Master Volume              " + to_string(_percent) + "%";
		mv_w = L"";
		mv_w.assign(mv.begin(), mv.end());
		//마스터 볼륨 조절
		if (IntersectRectToRect(&_temp, &_rcSettingChoice, &_rcSettings[2]))
		{
			if (KEYMANAGER->isOnceKeyDown('V') && _percent <= 100)
			{
				_percent += 5;
				_volume += 0.05;
				SOUNDMANAGER->setVolume(_volume);
			}
			if (_percent >= 105)
			{
				_volume = 0;
				_percent = 0;
				SOUNDMANAGER->setVolume(_volume);
			}
		}
			
		//기타 다른볼륨 조절은 할지말지
		bgv = "backGround Volume     " + to_string(_bgvPercent) + "%";
		bgv_w = L"";
		bgv_w.assign(bgv.begin(), bgv.end());
		if (IntersectRectToRect(&_temp, &_rcSettingChoice, &_rcSettings[3]))
		{
			if (KEYMANAGER->isOnceKeyDown('V') && _bgvPercent <= 100)
			{
				_bgvPercent += 5;
				_bgv += 0.05;
				SOUNDMANAGER->setVolume(_bgv);
			}
			if (_bgvPercent >= 105)
			{
				_volume = 0;
				_bgvPercent = 0;
				SOUNDMANAGER->setVolume(_bgv);
			}
		}

	}

	//메뉴로 돌아가기
	if (KEYMANAGER->isOnceKeyDown(VK_BACK) && (_openMenu == LOAD || _openMenu == ITEM || _openMenu == SETTINGS) )
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
		_load = IMAGEMANAGER->FindImage("load");
		_load->Render(Vector2(650, 400));
		_loadTop = IMAGEMANAGER->FindImage("loadTop");
		_loadTop->Render(Vector2(650, 120));
		
		D2DINS->GetInstance()->RenderText(300,95, L"어떤 파일에 저장하시겠습니까?", 30, D2DRenderer::DefaultBrush::White);

		D2DINS->GetInstance()->DrawRectangle(_rcLoadChoice, D2D1::ColorF::White, _rcAlpha, 1.0f);
		D2DINS->FillRectangle(_rcLoadChoice,D2D1::ColorF::Enum::WhiteSmoke,_rcAlpha / 5.5);
		for (int i = 0; i < 5; i++)
		{
			//D2DINS->GetInstance()->DrawRectangle(_rcLoad[i], D2D1::ColorF::White, _rcAlpha, 1.0f);
			D2DINS->GetInstance()->RenderText(380, 190 + i * 90, L"파일", 30, D2DRenderer::DefaultBrush::White);
		}
	}
	
	if (_openMenu == SETTINGS)
	{
		//for (int i = 0; i < 7; i++)
		//{
		//	D2DINS->GetInstance()->DrawRectangle(_rcSettings[i], D2D1::ColorF::White, _rcAlpha, 1.0f);
		//	D2DINS->FillRectangle(_rcSettings[i], D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		//}

		D2DINS->GetInstance()->RenderText(450, WINSIZEY / 2, L"항시 달리기                  OFF", 30, D2DRenderer::DefaultBrush::White);
		D2DINS->GetInstance()->RenderText(450, WINSIZEY / 2 + 50, L"전체 화면으로 기동      OFF", 30, D2DRenderer::DefaultBrush::White);
		D2DINS->GetInstance()->RenderText(450, WINSIZEY / 2 + 100, mv_w, 30, D2DRenderer::DefaultBrush::White);
		D2DINS->GetInstance()->RenderText(450, WINSIZEY / 2 + 150, bgv_w, 30, D2DRenderer::DefaultBrush::White);
		D2DINS->GetInstance()->RenderText(450, WINSIZEY / 2 + 200, L"환경음 음량      50%", 30, D2DRenderer::DefaultBrush::White);
		D2DINS->GetInstance()->RenderText(450, WINSIZEY / 2 + 250, L"효과음 음량      50%", 30, D2DRenderer::DefaultBrush::White);
		D2DINS->GetInstance()->RenderText(450, WINSIZEY / 2 + 300, L"타이틀 화면으로 돌아가기", 30, D2DRenderer::DefaultBrush::White);
		D2DINS->GetInstance()->DrawRectangle(_rcSettingChoice, D2D1::ColorF::White, _rcAlpha, 1.0f);
		D2DINS->FillRectangle(_rcSettingChoice, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);

	}

	if (_openMenu == ITEM)
	{
		_item = IMAGEMANAGER->FindImage("item");
		_item->Render(Vector2(650, 500));
		_itemTop = IMAGEMANAGER->FindImage("itemTop");
		_itemTop->Render(Vector2(650, 375));
	}

}

void playerMenu::openMenu()
{
	if (KEYMANAGER->isToggleKey('X'))
	{
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
		D2DINS->GetInstance()->RenderText(WINSIZEX /2 -100 , WINSIZEY / 2 + 160, L"비올라", RGB(255, 255, 255), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX /2 -100 , WINSIZEY / 2 + 200, L"나이", RGB(0, 0, 204), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX /2 +50 , WINSIZEY / 2 + 200, L"13", RGB(255, 255, 255), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX /2 +200 , WINSIZEY / 2 + 200, L"체력", RGB(0, 0, 204), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX /2 +300 , WINSIZEY / 2 + 200, L"10", RGB(255, 255, 255), 0.85f, 30);


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

}




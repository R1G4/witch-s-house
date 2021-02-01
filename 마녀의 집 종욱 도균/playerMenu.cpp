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
	//_x = WINSIZEX / 2;
	//_y = WINSIZEY / 2;
	_x = 160;
	_y = 505;
	_x2 = 650;
	_y2 = 230;
	_x3 = 630;
	_y3 = WINSIZEY/2 + 20;
	_rcAlpha = 1.0f;
	_rcAlphaChange = 0.03f;
	_rc = RectMakePivot(Vector2(_x, _y), Vector2(180, 45), Pivot::Center);
	_rc1 = RectMakePivot(Vector2(_x - 500, _y + 140), Vector2(120, 30), Pivot::Center);
	_rc2 = RectMakePivot(Vector2(_x - 500, _y + 200), Vector2(120, 30), Pivot::Center);
	_rc3 = RectMakePivot(Vector2(_x - 500, _y + 260), Vector2(120, 30), Pivot::Center);

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
	
	for (int i = 0; i < 6; i++)
	{
		_rcSettings[i] = RectMakePivot(Vector2(630, WINSIZEY / 2 + 20 + i * 50), Vector2(400, 50), Pivot::Center);
	}

	//wsprintf(str, "����%d", 1);


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


	if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && _rc.bottom <= _rcMenu[2].top && _openMenu != LOAD)
		_y += 50;
	if (KEYMANAGER->isOnceKeyDown(VK_UP) && _rc.top >= _rcMenu[0].bottom && _openMenu != LOAD)
		_y -= 50;

	//if (KEYMANAGER->isOnceKeyDown(VK_SPACE) && !_isCheck)
	//{
	//	_isCheck = true;
	//	_x = WINSIZEX / 2;
	//	_y = WINSIZEY / 2;
	//}

	//������, �ҷ�����, ����
	if (IntersectRectToRect(&_temp, &_rc, &_rcMenu[0]))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			_openMenu = ITEM;
		}
	}
	if (IntersectRectToRect(&_temp, &_rc, &_rcMenu[1]))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			_openMenu = LOAD;
		}
	}
	if (IntersectRectToRect(&_temp, &_rc, &_rcMenu[2]))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			_openMenu = SETTINGS;
		}
	}

	//�ҷ����� ȭ���϶�
	if (_openMenu == LOAD)
	{
		for (int i = 0; i < 5; i++)
		{
			if (KEYMANAGER->isOnceKeyDown('S') && _rcLoadChoice.bottom <= _rcLoad[4].top)
			{

				_y2 += 90;
			}
			if (KEYMANAGER->isOnceKeyDown('W') && _rcLoadChoice.top >= _rcLoad[0].bottom)
			{

				_y2 -= 90;
			}
		}
	}
	//���� ȭ���϶�
	if (_openMenu == SETTINGS)
	{
		if (KEYMANAGER->isOnceKeyDown('S') && _rcSettingChoice.bottom <= _rcSettings[5].top)
		{
			_y3 += 50;
		}
		if (KEYMANAGER->isOnceKeyDown('W') && _rcSettingChoice.top >= _rcSettings[0].bottom)
		{
			_y3 -= 50;
		}
	}
	//�޴��� ���ư���
	if (KEYMANAGER->isOnceKeyDown(VK_BACK) && (_openMenu == LOAD || _openMenu == ITEM || _openMenu == SETTINGS) )
	{
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
	//if (_isMenu)
	
	if (_openMenu == MENU && _openMenu != LOAD && _openMenu != ITEM && _openMenu != SETTINGS)
	{
		openMenu();
	}
	

	if (_openMenu == LOAD)
	{
		//cout << "lll" << endl;
		_load = IMAGEMANAGER->FindImage("load");
		_load->Render(Vector2(650, 400));
		_loadTop = IMAGEMANAGER->FindImage("loadTop");
		_loadTop->Render(Vector2(650, 120));
		
		D2DINS->GetInstance()->RenderText(300,95, L"� ���Ͽ� �����Ͻðڽ��ϱ�?", 30, D2DRenderer::DefaultBrush::White);

		D2DINS->GetInstance()->DrawRectangle(_rcLoadChoice, D2D1::ColorF::White, _rcAlpha, 1.0f);
		D2DINS->FillRectangle(_rcLoadChoice,D2D1::ColorF::Enum::WhiteSmoke,_rcAlpha / 5.5);
		for (int i = 0; i < 5; i++)
		{
			//D2DINS->GetInstance()->DrawRectangle(_rcLoad[i], D2D1::ColorF::White, _rcAlpha, 1.0f);
			D2DINS->GetInstance()->RenderText(380, 190 + i * 90, L"����", 30, D2DRenderer::DefaultBrush::White);
		}
	}
	
	if (_openMenu == SETTINGS)
	{
		//for (int i = 0; i < 6; i++)
		//{
		//	D2DINS->GetInstance()->DrawRectangle(_rcSettings[i], D2D1::ColorF::White, _rcAlpha, 1.0f);
		//	D2DINS->FillRectangle(_rcSettings[i], D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		//}

		D2DINS->GetInstance()->RenderText(470, WINSIZEY/2 , L"�׽� �޸���        OFF", 30, D2DRenderer::DefaultBrush::White);
		D2DINS->GetInstance()->RenderText(470, WINSIZEY/2 + 50, L"��ü ȭ������ �⵿      OFF", 30, D2DRenderer::DefaultBrush::White);
		D2DINS->GetInstance()->RenderText(470, WINSIZEY/2 + 100, L"������ ����      50%", 30, D2DRenderer::DefaultBrush::White);
		D2DINS->GetInstance()->RenderText(470, WINSIZEY/2 + 150, L"����� ����      50%", 30, D2DRenderer::DefaultBrush::White);
		D2DINS->GetInstance()->RenderText(470, WINSIZEY/2 + 200, L"ȯ���� ����      50%", 30, D2DRenderer::DefaultBrush::White);
		D2DINS->GetInstance()->RenderText(470, WINSIZEY/2 + 250, L"ȿ���� ����      50%", 30, D2DRenderer::DefaultBrush::White);
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

		//�޴� ĭ ��Ʈ �׽�Ʈ��
		//for (int i = 0; i < 3; i++)
		//{
		//	D2DINS->GetInstance()->DrawRectangle(_rcMenu[i], D2D1::ColorF::Yellow, _rcAlpha, 1.0f);
		//	D2DINS->FillRectangle(_rcMenu[i], D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		//}

		_menu = IMAGEMANAGER->FindImage("playerMenu");
		_menu->Render(Vector2(170, WINSIZEY / 2 + 200));

		_status = IMAGEMANAGER->FindImage("status");
		_status->Render(Vector2(WINSIZEX / 2 + 130, WINSIZEY / 2 + 200));
		D2DINS->GetInstance()->RenderText(WINSIZEX /2 -100 , WINSIZEY / 2 + 160, L"��ö�", RGB(255, 255, 255), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX /2 -100 , WINSIZEY / 2 + 200, L"����", RGB(0, 0, 204), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX /2 +50 , WINSIZEY / 2 + 200, L"13", RGB(255, 255, 255), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX /2 +200 , WINSIZEY / 2 + 200, L"ü��", RGB(0, 0, 204), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX /2 +300 , WINSIZEY / 2 + 200, L"10", RGB(255, 255, 255), 0.85f, 30);


		D2DINS->GetInstance()->RenderText(80, WINSIZEY / 2 + 130, L"����ǰ", RGB(255, 255, 255), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(80, WINSIZEY / 2 + 180, L"�ҷ�����", RGB(255, 255, 255), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(80, WINSIZEY / 2 + 230, L"����", RGB(255, 255, 255), 0.85f, 30);

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
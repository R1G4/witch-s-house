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

	//�޴� ���������� �Ʒ��� �����ǵ��ϴ� ŵ
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

	//�޴� ����������
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

	//������, �ҷ�����, ����

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE) && _isMenu)
	{
		//������
		if (IntersectRectToRect(&_temp, &_rc, &_rcMenu[0]))
		{
			SOUNDMANAGER->play("cursor", 0.5f);
			_openMenu = ITEM;
		}
		//�ҷ�����
		if (IntersectRectToRect(&_temp, &_rc, &_rcMenu[1]))
		{
			SOUNDMANAGER->play("cursor", 0.5f);
			_openMenu = LOAD;
		}
		//����
		if (IntersectRectToRect(&_temp, &_rc, &_rcMenu[2]))
		{
			SOUNDMANAGER->play("cursor", 0.5f);
			_openMenu = SETTINGS;
		}
	}

	//������â ȭ���϶�
	if (_openMenu == ITEM)
	{
		//������ Ŭ���� ����� �־��ֱ�
	}

	//�ҷ����� ȭ���϶�
	if (_openMenu == LOAD)
	{
		_load->update();
	}
	//���� ȭ���϶�
	if (_openMenu == SETTINGS)
	{
		_settings->update();
	}

	//�޴��� ���ư���
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
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 100, WINSIZEY / 2 + 160, L"��ö�", RGB(255, 255, 255), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 100, WINSIZEY / 2 + 200, L"����", RGB(0, 0, 204), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 50, WINSIZEY / 2 + 200, L"13", RGB(255, 255, 255), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 200, WINSIZEY / 2 + 200, L"ü��", RGB(0, 0, 204), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 300, WINSIZEY / 2 + 200, L"10", RGB(255, 255, 255), 0.85f, 30);


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
	else
	{
		_isMenu = false;
	}

}




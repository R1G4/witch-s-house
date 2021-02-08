#include "stdafx.h"
#include "menu.h"

menu::menu()
{
}

menu::~menu()
{
}

HRESULT menu::init()
{
	SOUNDMANAGER->play("main");
	_settings = new settings;
	_settings->init();

	//���� ��Ʈ �ʱ�ȭ
	_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 65), Vector2(220, 60), Pivot::Center);

	//���� ���� �ʱ�ȭ..
	_contents = NEW;
	_isClick = false;
	_volume = 0.5f;
	_sceneAlpha = 1.f;
	_rcAlpha = 1.f;
	_rcAlphaChange = 0.03f;

	//�޴� �ؽ�Ʈ ���� �ʱ�ȭ
	tagText textInfo;
	textInfo.x = WINSIZEX / 2 - 10;
	textInfo.y = WINSIZEY / 2;
	textInfo.size = 40;
	textInfo.text = L"ó������";
	_mText.insert(pair<CONTENTS, tagText>(NEW, textInfo));

	textInfo.text = L"����ؼ�";
	_mText.insert(pair<CONTENTS, tagText>(CONTINUE, textInfo));

	textInfo.text = L"��������";
	_mText.insert(pair<CONTENTS, tagText>(MAPTOOL, textInfo));

	textInfo.text = L"����";
	_mText.insert(pair<CONTENTS, tagText>(OPTION, textInfo));

	textInfo.text = L"������";
	_mText.insert(pair<CONTENTS, tagText>(END, textInfo));
	return S_OK;
}

void menu::release()
{
	SOUNDMANAGER->stop("main");
}

void menu::update()
{
	//�޴��� ������.
	openMenu();
}

void menu::openMenu()
{
	//�ҷ� �ʱ�ȭ
	SOUNDMANAGER->setVolume(_volume);

	//����Ű ����
	if (!_isClick && KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		_contents = (CONTENTS)(_contents == END ? 0 : (int)_contents + 1);
		SOUNDMANAGER->play("cursor", 0.5f);
	}

	if (!_isClick && KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		_contents = (CONTENTS)(_contents == NEW ? 3 : (int)_contents - 1);
		SOUNDMANAGER->play("cursor", 0.5f);
	}

	MENUSTATE _isMenuState;
	//���õ� �޴�(�׸�)�� ��Ʈ ��ġ ���� �� Ŭ�� �� ���� ��ȭ
	switch (_contents)
	{
	case menu::NEW:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 20), Vector2(220, 60), Pivot::Center);
		contentsCheck("��������");
		break;

	case menu::CONTINUE:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 90), Vector2(220, 60), Pivot::Center);
		//����� â�� ���� �糢�� �������� ���¸� �޾ƿ´�(�ڷΰ���, ����~)
		_isMenuState = STORAGEMANAGER->loadView();
		_isClick = _isMenuState ? true : false;
		break;

	case menu::MAPTOOL:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 160), Vector2(220, 60), Pivot::Center);
		contentsCheck("MapToolScene");
		break;

	case menu::OPTION:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 230), Vector2(220, 60), Pivot::Center);
		if (_isClick)
		{
			//����â�� ���� �糢�� �������� ���¸� �޾ƿ´�(�ڷΰ���, ����~)
			_isMenuState = _settings->settingOpen();
			if (_isMenuState == MENU_END || !_isMenuState)	_isClick = false;
		}
		//��Ű�ٿ��� ������ �����Ƿ� �����̷� ������
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			_isClick = true;
		break;

	case menu::END:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 300), Vector2(220, 60), Pivot::Center);
		if (_isClick)
		{
			//���� 1���϶�� �����Ѵ�.
			if (sceneAlphaChange())
				PostQuitMessage(0);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE) * 0x20)
			_isClick = true;
		break;
	}

	//���� Ű�� ������ �ʾҴٸ� ��Ʈ ��¦ ȿ��
	if (!_isClick)	rcAlphaChange();
}

void menu::contentsCheck(string key)
{	
	//Ŭ���� ���¶��
	if (_isClick)
	{	
		//sceneAlphaChange()�� �Լ��� ��ȯ���� bool�� Ư�� ������ �����ϸ� true�� ��ȯ��.
		//true�� ��ȯ �ϰ��� ��ü������ �ڿ�������
		if (sceneAlphaChange())
		{
			//���� ������
			SOUNDMANAGER->play("click", 0.5f);
			//���� ���� ���� ����
			SOUNDMANAGER->stop("main");
			//Ŭ�� ���¸� �ʱ�ȭ �Ѵ�.
			_isClick = false;
			//�� ����
			SCENEMANAGER->changeScene(key);
		}
	}
	//�����̽��ٸ� Ŭ�� ��
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE) * 0x20)
		_isClick = true;
}

void menu::rcAlphaChange()
{
	if (_rcAlpha >= 0.9f)
		_rcAlphaChange = -0.03f;

	if (_rcAlpha <= 0.35f)
		_rcAlphaChange = 0.03f;

	_rcAlpha += _rcAlphaChange;
}

bool menu::sceneAlphaChange()
{
	_sceneAlpha -= 0.01f;
	_rcAlpha -= 0.05f;

	if (_sceneAlpha <= 0.f)
		return true;
	else
		return false;
}

void menu::render()
{
	//��׶��� ���� �� ����~
	ImageManager::GetInstance()->FindImage("menu")->SetSize(Vector2(1280, 720));
	ImageManager::GetInstance()->FindImage("menu")->SetAlpha(_sceneAlpha);
	ImageManager::GetInstance()->FindImage("menu")->Render(Vector2(WINSIZEX / 2, WINSIZEY / 2));

	if (!_isClick)
	{
		//��׶��� �÷� ��Ʈ
		D2DINS->FillRectangle(_rcSelected, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 3.5);
		//�׵θ� �÷� ��Ʈ
		D2DINS->DrawRectangle(_rcSelected, D2D1::ColorF::Enum::White, _rcAlpha, 3.f);
		for (int i = 0; i < _mText.size(); i++)
		{
			CONTENTS sel = (CONTENTS)i;
			D2DINS->RenderText
			(
				_mText.find(sel)->second.x - _mText.find(sel)->second.size * 2,
				_mText.find(sel)->second.y + i * 70,
				_mText.find(sel)->second.text,
				RGB(255, 255, 255),
				_sceneAlpha,
				_mText.find(sel)->second.size,
				DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_TRAILING
				//��Ʈ ���� �𸣰ٽ�..
			);
		}
	}
	else
	{
		switch (_contents)
		{
		case menu::CONTINUE:
			STORAGEMANAGER->render();
			break;
		case menu::OPTION:
			_settings->render();
			break;
		}
	}
}
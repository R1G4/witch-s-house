#include "stdafx.h"
#include "inMenu.h"


inMenu::inMenu() :
_rcAlpha(1.f),
_rcAlphaChange(0.03), 
_sceneAlpha(0),
_isConnecting(0),
_isOpen(0)
{
}


inMenu::~inMenu()
{
}

HRESULT inMenu::init() 
{
	//������ �ʱ�ȭ
	_settings = new settings;
	_settings->init();

	_rcSelected = RectMakePivot(Vector2(160, 505), Vector2(180, 45), Pivot::Center);
	_contents = ITEM;

	_rcAlpha = 1.0f;
	_rcAlphaChange = 0.03f;

	for (int i = 0; i < 3; i++)
	{
		tagTextInfo _textInfo;
		_textInfo.vec = Vector2(80, (WINSIZEY / 2 + 135) + i * 50);
		_textInfo.text = i ==  0 ? L"����ǰ" : i == 1 ? L"�ҷ�����" : L"����";
		_textInfo.rc = RectMakePivot(Vector2(168, 510  + (i * 50)), Vector2(180, 45), Pivot::Center);

		_mText.insert(pair<CONTENTS, tagTextInfo>((CONTENTS)i, _textInfo));
	}
	return S_OK;
}

void inMenu::release()
{
}

void inMenu::rcAlphaChange()
{
	if (_rcAlpha >= 0.9f)
		_rcAlphaChange = -0.03f;

	if (_rcAlpha <= 0.35f)
		_rcAlphaChange = 0.03f;

	_rcAlpha += _rcAlphaChange;
}

void inMenu::render()
{	
	if (!_isOpen) return;

	if (_isConnecting)
	{
		//���õǾ� �ִ� ������
		switch (_contents)
		{
			case ITEM:
				ITEMMANAGER->render();
				break;
			case LOAD:
				STORAGEMANAGER->render();
				break;
			case SETTINGS:
				_settings->render();
				break;
		}
	}
	else
	{
		IMAGEMANAGER->FindImage("playerMenu")->Render(Vector2(170, WINSIZEY / 2 + 200));
		IMAGEMANAGER->FindImage("status")->Render(Vector2(WINSIZEX / 2 + 130, WINSIZEY / 2 + 200));

		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 100, WINSIZEY / 2 + 160, L"��ö�", RGB(255, 255, 255), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 100, WINSIZEY / 2 + 200, L"����", RGB(0, 0, 204), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 50, WINSIZEY / 2 + 200, L"13", RGB(255, 255, 255), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 200, WINSIZEY / 2 + 200, L"ü��", RGB(0, 0, 204), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 300, WINSIZEY / 2 + 200, L"10", RGB(255, 255, 255), 0.85f, 30);

		//��׶��� �÷� ��Ʈ
		D2DINS->FillRectangle
		(
			_mText.find(_contents)->second.rc,
			D2D1::ColorF::Enum::WhiteSmoke,
			_rcAlpha / 3.5
		);
		for (int i = 0; i < _mText.size(); i++)
		{
			CONTENTS sel = (CONTENTS)i;
			wstring temp = _mText.find(sel)->second.text;
			tagTextInfo temp2 = _mText.find(sel)->second;
			D2DINS->RenderText
			(
				_mText.find(sel)->second.vec.x,
				_mText.find(sel)->second.vec.y,
				_mText.find(sel)->second.text,
				RGB(255, 255, 255),
				 0.85f,
				30,
				DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_TRAILING
			);
		}
		//�׵θ� �÷� ��Ʈ
		D2DINS->DrawRectangle
		(
			_mText.find(_contents)->second.rc,
			D2D1::ColorF::Enum::White,
			_rcAlpha,
			3.f
		);
	}
}

bool inMenu::openInMenu()
{
	//�÷��̾ ����ȭ��(�޴�)���� �����ϰ� �ִٸ�
	//�ٷ� false�� ��ȯ�Ѵ�.
	if (SCENEMANAGER->getKeyCheck("����ȭ��"))
		return false;

	//�ٸ� �������� �̿��ϰ� �ִٸ� �����Ѵ�.
	//XŰ�� ���� �� ��������� �� ������ �޴� ���۰� ���Ḧ �����Ѵ�.
	if (!_isConnecting && KEYMANAGER->isOnceKeyDown('X'))	_isOpen = !_isOpen;

	//�޴��� ���� ��� false�� ��ȯ�Ѵ�,
	if(!_isOpen)	return _isOpen;

	//�����ϴ� �������� ��Ʈ�� �Ѵ�.
	selectedContents();

	//Ŭ���� ���¶�� ����~
	if(_isConnecting);	connect();

	//��Ʈ ���� ���� (����ȿ��)
	rcAlphaChange();

	return _isOpen;
}

void inMenu::selectedContents()
{
	if (_isConnecting)	return;

	////����Ű ����
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		SOUNDMANAGER->play("cursor", 0.5f);
		_contents = (CONTENTS)(_contents + 1 < 3 ? _contents + 1 : _contents);
	}

	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		SOUNDMANAGER->play("cursor", 0.5f);
		_contents = (CONTENTS)(_contents - 1 > -1 ? _contents - 1 : _contents);
	}

	if (_contents != LOAD && KEYMANAGER->isOnceKeyDown(VK_SPACE/*'Z'*/))
	{
		SOUNDMANAGER->play("cursor", 0.5f);
		_isConnecting = true;
		//cout << _isConnecting;
	}
}

void inMenu::connect()
{
	MENUSTATE _isMenuState;

	//���õǾ� �ִ� ������
	switch (_contents)
	{
		case ITEM:
			//������ â���� �����̽��� ������ ��ü �޴� â ����
			//������ â���� xŰ ������ �ڷΰ���
			//�������� ���� ���� �ʴ´ٸ� �ڷΰ��⸸ ����
			_isMenuState = ITEMMANAGER->itemOpen();

			if (!_isMenuState)	_isConnecting = false;
			if (_isMenuState == MENU_END)
			{
				_isOpen = false;
				_isConnecting = false;
			}
			break;
		case LOAD:
			//�ҷ����� â���� xŰ ������ �ڷΰ���
			//�������� ���� ���� �ʴ´ٸ� �ڷΰ��⸸ ����
 			_isMenuState = STORAGEMANAGER->loadView();

			cout << _isConnecting;
			if(_isMenuState == MENU_PROGRESS)	_isConnecting = true;
			if (!_isMenuState)	_isConnecting = false;
			if (_isMenuState == MENU_END)
			{
				_isOpen = false;
				_isConnecting = false;

				//���⼭ �ҷ����� �ƴϸ� �Ŵ������� �ҷ�����?
			}
			break;
		case SETTINGS:	
			_isMenuState = _settings->settingOpen();

			if (!_isMenuState)	_isConnecting = false;
			if (_isMenuState == MENU_END)
			{
				_isOpen = false;
				_isConnecting = false;
				SOUNDMANAGER->stop("main");
				SCENEMANAGER->changeScene("����ȭ��");
			}
			break;
	}
}
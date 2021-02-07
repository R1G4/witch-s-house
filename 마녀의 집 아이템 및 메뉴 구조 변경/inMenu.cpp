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
	//컨텐츠 초기화
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
		_textInfo.text = i ==  0 ? L"소지품" : i == 1 ? L"불러오기" : L"설정";
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
		//선택되어 있는 컨텐츠
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

		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 100, WINSIZEY / 2 + 160, L"비올라", RGB(255, 255, 255), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 100, WINSIZEY / 2 + 200, L"나이", RGB(0, 0, 204), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 50, WINSIZEY / 2 + 200, L"13", RGB(255, 255, 255), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 200, WINSIZEY / 2 + 200, L"체력", RGB(0, 0, 204), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 300, WINSIZEY / 2 + 200, L"10", RGB(255, 255, 255), 0.85f, 30);

		//백그라운드 컬러 렉트
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
		//테두리 컬러 렉트
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
	//플레이어가 시작화면(메뉴)에서 조작하고 있다면
	//바로 false를 반환한다.
	if (SCENEMANAGER->getKeyCheck("시작화면"))
		return false;

	//다른 컨텐츠를 이용하고 있다면 무시한다.
	//X키를 누를 시 토글형식의 불 변수로 메뉴 시작과 종료를 구분한다.
	if (!_isConnecting && KEYMANAGER->isOnceKeyDown('X'))	_isOpen = !_isOpen;

	//메뉴가 닫힌 경우 false를 반환한다,
	if(!_isOpen)	return _isOpen;

	//선택하는 컨텐츠를 컨트롤 한다.
	selectedContents();

	//클릭한 상태라면 연결~
	if(_isConnecting);	connect();

	//렉트 투명도 조절 (선택효과)
	rcAlphaChange();

	return _isOpen;
}

void inMenu::selectedContents()
{
	if (_isConnecting)	return;

	////방향키 조작
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

	//선택되어 있는 컨텐츠
	switch (_contents)
	{
		case ITEM:
			//아이템 창에서 스페이스바 누르면 전체 메뉴 창 종료
			//아이템 창에서 x키 누르면 뒤로가기
			//아이템이 존재 하지 않는다면 뒤로가기만 가능
			_isMenuState = ITEMMANAGER->itemOpen();

			if (!_isMenuState)	_isConnecting = false;
			if (_isMenuState == MENU_END)
			{
				_isOpen = false;
				_isConnecting = false;
			}
			break;
		case LOAD:
			//불러오기 창에서 x키 누르면 뒤로가기
			//아이템이 존재 하지 않는다면 뒤로가기만 가능
 			_isMenuState = STORAGEMANAGER->loadView();

			cout << _isConnecting;
			if(_isMenuState == MENU_PROGRESS)	_isConnecting = true;
			if (!_isMenuState)	_isConnecting = false;
			if (_isMenuState == MENU_END)
			{
				_isOpen = false;
				_isConnecting = false;

				//여기서 불러올지 아니면 매니저에서 불러올지?
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
				SCENEMANAGER->changeScene("시작화면");
			}
			break;
	}
}
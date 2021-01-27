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
	//해당 컨텐츠는 처음부터
	_contents = NEW;
	_isClick = false;

	//선택 렉트 초기화
	_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 65), Vector2(220, 60), Pivot::Center);

	//투명도 초기화
	_sceneAlpha = 1.f;
	_rcAlpha = 1.f;
	_rcAlphaChange = 0.03f;

	//각 메뉴의 컨텐츠 정보 초기화
	setText();

	return S_OK;
}

void menu::setText()
{	
	//메뉴 텍스트 정보 초기화
	tagText textInfo;
	textInfo.x = WINSIZEX / 2 - 10;
	textInfo.y = WINSIZEY / 2 + 35;
	textInfo.size = 40;
	textInfo.text = L"처음부터";
	_mText.insert(pair<CONTENTS, tagText>(NEW, textInfo));

	textInfo.text = L"계속해서";
	_mText.insert(pair<CONTENTS, tagText>(CONTINUE, textInfo));

	textInfo.text = L"설정";
	_mText.insert(pair<CONTENTS, tagText>(OPTION, textInfo));

	textInfo.text = L"끝내기";
	_mText.insert(pair<CONTENTS, tagText>(END, textInfo));
}

void menu::release()
{
}

void menu::update()
{
	//방향키 조작
	if (!_isClick && KEYMANAGER->isOnceKeyDown(VK_DOWN))
		_contents = (CONTENTS)(_contents == END ? 0 : (int)_contents + 1);

	if (!_isClick && KEYMANAGER->isOnceKeyDown(VK_UP))
		_contents = (CONTENTS)(_contents == NEW ? 3 : (int)_contents - 1);

	//선택 키를 누르지 않았다면 렉트 반짝 효과
	if (!_isClick)	rcAlphaChange();

	//선택된 메뉴(항목)에 렉트 위치 변경
	switch (_contents)
	{
		case menu::NEW:
			_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 55), Vector2(220, 60), Pivot::Center);
			if (_isClick)
			{
				if (sceneAlphaChange())
					SCENEMANAGER->changeScene("레벨선택");
			}
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
				_isClick = true;
			break;

		case menu::CONTINUE:
			_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 125), Vector2(220, 60), Pivot::Center);
			if (_isClick)
				SCENEMANAGER->changeScene("계속하기");

			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
				_isClick = true;
			break;

		case menu::OPTION:
			_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 195), Vector2(220, 60), Pivot::Center);
			break;

		case menu::END:
			_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 265), Vector2(220, 60), Pivot::Center);
			if (_isClick)
			{
				if (sceneAlphaChange())
					PostQuitMessage(0);
			}
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
				_isClick = true;
			break;
	}
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
	//백그라운드 셋팅 및 랜더~
	ImageManager::GetInstance()->FindImage("menu")->SetSize(Vector2(1280, 720));
	ImageManager::GetInstance()->FindImage("menu")->SetAlpha(_sceneAlpha);
	ImageManager::GetInstance()->FindImage("menu")->Render(Vector2(WINSIZEX / 2, WINSIZEY / 2));

	//백그라운드 컬러 렉트
	D2DINS->FillRectangle
	(
		_rcSelected,
		D2D1::ColorF::Enum::WhiteSmoke,
		_rcAlpha / 3.5
	);

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
			//폰트 아직 모르겟숴..
		);
	}

	//테두리 컬러 렉트
	D2DINS->DrawRectangle
	(
		_rcSelected,
		D2D1::ColorF::Enum::White,
		_rcAlpha,
		3.f
	);
}
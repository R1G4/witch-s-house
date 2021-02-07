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
	_settings = new settings;
	_settings->init();

	//선택 렉트 초기화
	_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 65), Vector2(220, 60), Pivot::Center);

	//각종 변수 초기화..
	_contents = NEW;
	_isClick = false;
	_volume = 0.5f;
	_sceneAlpha = 1.f;
	_rcAlpha = 1.f;
	_rcAlphaChange = 0.03f;

	//메뉴 텍스트 정보 초기화
	tagText textInfo;
	textInfo.x = WINSIZEX / 2 - 10;
	textInfo.y = WINSIZEY / 2;
	textInfo.size = 40;
	textInfo.text = L"처음부터";
	_mText.insert(pair<CONTENTS, tagText>(NEW, textInfo));

	textInfo.text = L"계속해서";
	_mText.insert(pair<CONTENTS, tagText>(CONTINUE, textInfo));

	textInfo.text = L"맵툴설정";
	_mText.insert(pair<CONTENTS, tagText>(MAPTOOL, textInfo));

	textInfo.text = L"설정";
	_mText.insert(pair<CONTENTS, tagText>(OPTION, textInfo));

	textInfo.text = L"끝내기";
	_mText.insert(pair<CONTENTS, tagText>(END, textInfo));
	return S_OK;
}

void menu::release()
{
}

void menu::update()
{
	//메뉴가 열린다.
	openMenu();

#pragma region 혹시몰라 내가 건들기 전에꺼는 냅두게
	//SOUNDMANAGER->setVolume(_volume);
	////방향키 조작
	//if (!_isClick && KEYMANAGER->isOnceKeyDown(VK_DOWN))
	//{
	//	_contents = (CONTENTS)(_contents == END ? 0 : (int)_contents + 1);
	//	SOUNDMANAGER->play("cursor", 0.5f);
	//}

	//if (!_isClick && KEYMANAGER->isOnceKeyDown(VK_UP))
	//{
	//	_contents = (CONTENTS)(_contents == NEW ? 3 : (int)_contents - 1);
	//	SOUNDMANAGER->play("cursor", 0.5f);
	//}

	//if (!_isClick)
	//{
	//	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	//	{
	//		//새로하기(난이도 선택으로)
	//		if (_contents == NEW)
	//		{

	//			_isClick = true;
	//			SOUNDMANAGER->play("click", 0.5f);
	//			sceneAlphaChange();
	//			SCENEMANAGER->changeScene("레벨선택");
	//			SOUNDMANAGER->stop("main");
	//		}
	//		//옵션 설정
	//		if (_contents == OPTION)
	//		{
	//			_openOption = true;
	//		}
	//		//맵툴설정으로
	//		if (_contents == MAPTOOL)
	//		{
	//			_isClick = true;
	//			SOUNDMANAGER->play("click", 0.5f);
	//			//sceneAlphaChange();
	//			SCENEMANAGER->changeScene("MapToolScene");
	//			SOUNDMANAGER->stop("main");
	//		}
	//	}

	//	if (_openOption)
	//	{
	//		_settings->update();
	//		if (KEYMANAGER->isOnceKeyDown(VK_BACK))
	//		{
	//			_openOption = false;
	//		}
	//	}
	//}
	////메인메뉴 설정 창


	//if (_isClick)
	//{
	//	sceneAlphaChange();
	//}

	//if (!_isClick)
	//{
	//	rcAlphaChange();
	//}
	////else
	////{
	////	sceneAlphaChange();
	////}

	////선택된 메뉴(항목)에 렉트 위치 변경
	//switch (_contents)
	//{
	//case menu::NEW:
	//	_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 20), Vector2(220, 60), Pivot::Center);
	//	break;
	//case menu::CONTINUE:
	//	_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 90), Vector2(220, 60), Pivot::Center);
	//	break;
	//case menu::MAPTOOL:
	//	_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 160), Vector2(220, 60), Pivot::Center);
	//	break;
	//case menu::OPTION:
	//	_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 230), Vector2(220, 60), Pivot::Center);
	//	break;
	//case menu::END:
	//	_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 300), Vector2(220, 60), Pivot::Center);
	//	if (_isClick)
	//	{
	//		//투명도 1이하라면 종료한다.
	//		if (sceneAlphaChange())
	//			PostQuitMessage(0);
	//	}
	//	//원키다운이 먹히지 않으므로 스테이로 수정함
	//	if (KEYMANAGER->isStayKeyDown(VK_SPACE))
	//		_isClick = true;
	//	break;
	//}

	////선택 키를 누르지 않았다면 렉트 반짝 효과
	////if (!_isClick)
	////{
	////	rcAlphaChange();
	////}
	////선택 키를 눌렀다면 백그라운드 및 이미지 투명도 조절
	////else
	////{
	////	
	////	if (sceneAlphaChange())
	////	{
	////		SCENEMANAGER->changeScene("레벨선택");
	////	}
	////	//투명도 조절 후 다음씬으로 휙
	////}
#pragma endregion
}

void menu::openMenu()
{
	//불륨 초기화
	SOUNDMANAGER->setVolume(_volume);

	//방향키 조작
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
	//선택된 메뉴(항목)에 렉트 위치 변경 및 클릭 시 상태 변화
	switch (_contents)
	{
	case menu::NEW:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 20), Vector2(220, 60), Pivot::Center);
		contentsCheck("레벨선택");
		break;

	case menu::CONTINUE:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 90), Vector2(220, 60), Pivot::Center);
		//if (_isClick)
		{
			//저장소 창을 열어 재끼고 컨텐츠의 상태를 받아온다(뒤로가기, 진행~)
			_isMenuState = STORAGEMANAGER->loadView();
			_isClick = _isMenuState ? true : false;
		}
		//if (_isClick)
		//{
		//   //저장소 창을 열어 재끼고 컨텐츠의 상태를 받아온다(뒤로가기, 진행~)
		//   _isMenuState = STORAGEMANAGER->loadView();
		//   //if(_isMenuState == MENU_END)
		//   //여기서 불러온다? 혹은 매니저에서 불러와야하나 생각좀

		//   if (!_isMenuState)   _isClick = false;
		//}
		////원키다운이 먹히지 않으므로 스테이로 수정함
		//if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		//   _isClick = true;
		break;

	case menu::MAPTOOL:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 160), Vector2(220, 60), Pivot::Center);
		contentsCheck("MapToolScene");
		break;

	case menu::OPTION:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 230), Vector2(220, 60), Pivot::Center);
		if (_isClick)
		{
			//설정창을 열어 재끼고 컨텐츠의 상태를 받아온다(뒤로가기, 진행~)
			_isMenuState = _settings->settingOpen();
			if (_isMenuState == MENU_END || !_isMenuState)	_isClick = false;
		}
		//원키다운이 먹히지 않으므로 스테이로 수정함
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			_isClick = true;
		break;

	case menu::END:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 300), Vector2(220, 60), Pivot::Center);
		if (_isClick)
		{
			//투명도 1이하라면 종료한다.
			if (sceneAlphaChange())
				PostQuitMessage(0);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE) * 0x20)
			_isClick = true;
		break;
	}

	//선택 키를 누르지 않았다면 렉트 반짝 효과
	if (!_isClick)	rcAlphaChange();
}

void menu::contentsCheck(string key)
{	
	//클릭한 상태라면
	if (_isClick)
	{	
		//sceneAlphaChange()이 함수는 반환형이 bool임 특정 투명도에 도달하면 true를 반환함.
		//true를 반환 하고나서 씬체인지가 자연스러움
		if (sceneAlphaChange())
		{
			//뮤직 스따뜨
			SOUNDMANAGER->play("click", 0.5f);
			//기존 메인 뮤직 스땁
			SOUNDMANAGER->stop("main");
			//클릭 상태를 초기화 한다.
			_isClick = false;
			//씬 변경
			SCENEMANAGER->changeScene(key);
		}
	}
	//스페이스바를 클릭 시
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
	//백그라운드 셋팅 및 랜더~
	ImageManager::GetInstance()->FindImage("menu")->SetSize(Vector2(1280, 720));
	ImageManager::GetInstance()->FindImage("menu")->SetAlpha(_sceneAlpha);
	ImageManager::GetInstance()->FindImage("menu")->Render(Vector2(WINSIZEX / 2, WINSIZEY / 2));

	if (!_isClick)
	{
		//백그라운드 컬러 렉트
		D2DINS->FillRectangle(_rcSelected, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 3.5);
		//테두리 컬러 렉트
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
				//폰트 아직 모르겟숴..
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
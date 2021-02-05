#include "stdafx.h"
#include "gardenToBoss_5f.h"
#include "Player.h"

HRESULT gardenToBoss_5f::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_player->setDirec(_chrdirection);

	//타일 불러오기
	load(_location);

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	fifthFloorStage::init();

	// 다이얼로그 저장

	return S_OK;
}

void gardenToBoss_5f::release()
{
}

void gardenToBoss_5f::update()
{
	if (!_isStopToRead)
	{
		fifthFloorStage::update();
		setFrameIndex();

		//카메라 관련 업데이트
		cameraUpdate();
		setTrigger();
	}
}

void gardenToBoss_5f::render()
{	
	//배경 랜더
	IMAGEMANAGER->FindImage("배경72")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경72"),
		Vector2(IMAGEMANAGER->FindImage("배경72")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("배경72")->GetSize().y / 2));

	fifthFloorStage::render();

	if (_isStopToRead)
		renderText();
}

void gardenToBoss_5f::Collision()
{
}

void gardenToBoss_5f::load(LOCATION _location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/5f_garden_to_boss.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute != PLAYER) continue;

		//초기 위치를 잡아준다.
		switch (_location)
		{
		case LOCATION_DEFAULT: default:
			_player->setStart(i % TILEX, i / TILEX);
			break;
		}
		break;
	}
	CloseHandle(file);
}

void gardenToBoss_5f::setTrigger()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// 클릭행동 트리거
	{
		if (IntersectRectToRect(&_tiles[BOOK].rc, &_player->getSearchRc()))
		{
			if (!_isStopToRead)
				_vScript = TXTDATA->txtLoad("dialog/5f_gardenToBossBook.txt", "\n");
			_isStopToRead = true;
		}
		if (IntersectRectToRect(&_tiles[NEKO].rc, &_player->getSearchRc()))
		{
			if (!_isStopToRead)
				_vScript = TXTDATA->txtLoad("dialog/5f_gardenToBossNeko.txt", "\n");
			_isStopToRead = true;
			cout << "세이브!" << endl;
		}
	}

	if (IntersectRectToRect(&_tiles[DOORTOBOSS].rc, &_player->getPlayerFrc()))
	{
		_isChangeScene = true;
		_vFrameTile[0].isTrigger = true;
		sceneChange("BossStage");
		fifthFloorStage::release();
		cout << "보스로!" << endl;
	}
	if (IntersectRectToRect(&_tiles[DOORTOGARDEN].rc, &_player->getPlayerFrc()))
	{
		_isChangeScene = true;
		sceneChange("garden_5f", CHRDIREC_DOWN, LOCATION_2);
		cout << "가든으로!" << endl;
	}
}

void gardenToBoss_5f::renderText()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		if (_string_count < _vScript.size())
		{
			_string_count++;
			_i_word = 0;
			if (_string_count >= _vScript.size())
			{
				_isStopToRead = false;
				_string_count = 0;
				return;
			}
		}
	}

	D2DINS->FillRectangle(RectMakePivot(Vector2(0, WINSIZEY / 2 - 100), Vector2(WINSIZEX, 200), Pivot::LeftTop), D2D1::ColorF::Black, 0.5);
	
	char str[256];
	wstring wstr;

	strcpy_s(str, _vScript[_string_count].c_str());

	_count_dia++;
	if (_count_dia % 5 == 0 && _i_word <= strlen(str))	// 한줄 출력시 한글자씩 출력하게하는 함수
	{
		_i_word = _i_word + 2;
		strncpy_s(str, str, _i_word);
		wstr = stringToWstring(str);
		_count_dia = 0;
	}
	else if (_i_word >= strlen(str))
	{
		strncpy_s(str, str, _i_word);
		wstr = stringToWstring(str);
		_count_dia = 0;
	}
	else
	{
		strncpy_s(str, str, _i_word);
		wstr = stringToWstring(str);
	}

	D2DINS->RenderTextField(WINSIZEX / 2 - 200, WINSIZEY / 2 - 100, wstr, RGB(255, 255, 255), 20, 400, 200, 1.0);
}

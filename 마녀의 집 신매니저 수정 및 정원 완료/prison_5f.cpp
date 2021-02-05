#include "stdafx.h"
#include "prison_5f.h"
#include "Player.h"

HRESULT prison_5f::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_player->setDirec(_chrdirection);

	//타일 불러오기
	load(_location);

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	fifthFloorStage::init();

	return S_OK;
}

void prison_5f::release()
{
}

void prison_5f::update()
{
	if (!_isStopToRead)
	{
		fifthFloorStage::update();
		//_player->update();
		setFrameIndex();

		//카메라 관련 업데이트
		cameraUpdate();

		setTrigger();
	}
	else
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// 클릭행동 트리거
		{
			_isStopToRead = TEXTMANAGER->setNextScript(true);
		}
	}

	//cout << "x : " << (int)(_player->getPlayerLocX()) / TILESIZE << " y : " << (int)(_player->getPlayerLocY()) / TILESIZE << endl;
}

void prison_5f::render()
{	
	//배경 랜더
	IMAGEMANAGER->FindImage("배경65")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경65"),
		Vector2(IMAGEMANAGER->FindImage("배경65")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("배경65")->GetSize().y / 2));

	fifthFloorStage::render();

	if (_isStopToRead)
		TEXTMANAGER->renderText();
}

void prison_5f::Collision()
{
}

void prison_5f::load(LOCATION _location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/5f_prison.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute != PLAYER) continue;

		//초기 위치를 잡아준다.
		switch (_location)
		{
		case LOCATION_1:
			_player->setStart((DOORTOPRISONSKUL - 1) % TILEX, (DOORTOPRISONSKUL - 1) / TILEX);
			break;
		case LOCATION_2:
			_player->setStart((DOORTOPRISONWELL + TILEX) % TILEX, (DOORTOPRISONWELL + TILEX) / TILEX);
			break;
		case LOCATION_DEFAULT: default:
			_player->setStart(i % TILEX, i / TILEX);
			break;
		}
		break;
	}
	CloseHandle(file);
}

void prison_5f::setTrigger()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// 클릭행동 트리거
	{
		_isStopToRead = TEXTMANAGER->setNextScript(true);

		if (IntersectRectToRect(&_tiles[R_FLOWER_1].rc, &_player->getSearchRc()))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_flower_1.txt");
			cout << "꽃1!" << endl;
			_isStopToRead = true;
		}
		if (IntersectRectToRect(&_tiles[R_FLOWER_2].rc, &_player->getSearchRc()))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_flower_1.txt");
			cout << "꽃2!" << endl;
			_isStopToRead = true;
		}
		if (IntersectRectToRect(&_tiles[R_FLOWER_3].rc, &_player->getSearchRc()))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_flower_2.txt");
			cout << "꽃3!" << endl;
			_isStopToRead = true;
		}
		if (IntersectRectToRect(&_tiles[BOOK].rc, &_player->getSearchRc()))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_book.txt");
			cout << "책!" << endl;
			_isStopToRead = true;
		}
		if (IntersectRectToRect(&_tiles[PAPER].rc, &_player->getSearchRc()))
		{
			cout << "힌트!" << endl;
		}
		if (IntersectRectToRect(&_tiles[CAGE].rc, &_player->getSearchRc()))
		{
			cout << "새장!" << endl;
		}
	}

	if (IntersectRectToRect(&_tiles[SKUL].rc, &_player->getPlayerFrc()))
	{
		cout << "해골!" << endl;
	}
	if (IntersectRectToRect(&_tiles[DOORTOGARDEN].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOGARDEN + TILEX].rc, &_player->getPlayerFrc()))
	{
		_isChangeScene = true;
		sceneChange("garden_5f", CHRDIREC_LEFT, LOCATION_3);
		cout << "정원으로!" << endl;
	}
	if (IntersectRectToRect(&_tiles[DOORTOPRISONWELL].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISONWELL + 1].rc, &_player->getPlayerFrc()))
	{
		_isChangeScene = true;
		sceneChange("prison_well_5f", CHRDIREC_UP, LOCATION_DEFAULT);
		cout << "우물로!" << endl;
	}
	if (IntersectRectToRect(&_tiles[DOORTOPRISONSKUL].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISONSKUL + TILEX].rc, &_player->getPlayerFrc()))
	{
		_isChangeScene = true;
		sceneChange("prison_skul_5f", CHRDIREC_RIGHT, LOCATION_DEFAULT);
		cout << "해골방으로!" << endl;
	}
}

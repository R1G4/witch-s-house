#include "stdafx.h"
#include "prison_5f_skul.h"
#include "Player.h"

HRESULT prison_5f_skul::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_player->setDirec(_chrdirection);

	//타일 불러오기
	load(_location);

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	fifthFloorStage::init();

	return S_OK;
}

void prison_5f_skul::release()
{
}

void prison_5f_skul::update()
{
	fifthFloorStage::update();
	//_player->update();
	setFrameIndex();

	//카메라 관련 업데이트
	cameraUpdate();
	setTrigger();
	//cout << "x : " << (int)(_player->getPlayerLocX()) / TILESIZE << " y : " << (int)(_player->getPlayerLocY()) / TILESIZE << endl;
}

void prison_5f_skul::render()
{	
	//배경 랜더
	IMAGEMANAGER->FindImage("배경67")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경67"),
		Vector2(IMAGEMANAGER->FindImage("배경67")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("배경67")->GetSize().y / 2));

	fifthFloorStage::render();
}

void prison_5f_skul::Collision()
{
}

void prison_5f_skul::load(LOCATION _location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/5f_prison_1.map", GENERIC_READ, NULL, NULL,
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

void prison_5f_skul::setTrigger()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// 클릭행동 트리거
	{
		if (IntersectRectToRect(&_tiles[SKUL_1].rc, &_player->getSearchRc()))
		{
			cout << "해골1!" << endl;
		}
		if (IntersectRectToRect(&_tiles[SKUL_2].rc, &_player->getSearchRc()))
		{
			cout << "해골2!" << endl;
		}
		if (IntersectRectToRect(&_tiles[SKUL_3].rc, &_player->getSearchRc()))
		{
			cout << "해골3!" << endl;
		}
		if (IntersectRectToRect(&_tiles[SKUL_4].rc, &_player->getSearchRc()))
		{
			cout << "해골4!" << endl;
		}
		if (IntersectRectToRect(&_tiles[SKUL_5].rc, &_player->getSearchRc()))
		{
			cout << "해골5!" << endl;
		}
		if (IntersectRectToRect(&_tiles[LEVER].rc, &_player->getSearchRc()))
		{
			cout << "레버!" << endl;
		}
	}

	if (IntersectRectToRect(&_tiles[DOORTOPRISON].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISON + 1].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISON - 1].rc, &_player->getPlayerFrc()))
	{
		_isChangeScene = true;
		sceneChange("prison_5f", CHRDIREC_LEFT, LOCATION_1);
		cout << "감옥으로!" << endl;
	}
}

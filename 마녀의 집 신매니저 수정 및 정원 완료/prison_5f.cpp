#include "stdafx.h"
#include "prison_5f.h"
#include "Player.h"

HRESULT prison_5f::init()
{
	_player->setDirec(CHRDIREC_RIGHT);

	//타일 불러오기
	load();

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	fifthFloorStage::init();

	return S_OK;
}

void prison_5f::release()
{
	_player->release();
	_vFrameTile.clear();
}

void prison_5f::update()
{
	fifthFloorStage::update();
	//_player->update();
	setFrameIndex();

	//카메라 관련 업데이트
	cameraUpdate();

	setTrigger();
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
}

void prison_5f::Collision()
{
}

void prison_5f::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/5f_prison.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute == PLAYER)
		{
			_player->setStart(i % TILEX + 1, i / TILEX);

			break;
		}
	}
	CloseHandle(file);
}

void prison_5f::setTrigger()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// 클릭행동 트리거
	{
		if (IntersectRectToRect(&_tiles[R_FLOWER_1].rc, &_player->getSearchRc()))
		{
			cout << "꽃1!" << endl;
		}
		if (IntersectRectToRect(&_tiles[R_FLOWER_2].rc, &_player->getSearchRc()))
		{
			cout << "꽃2!" << endl;
		}
		if (IntersectRectToRect(&_tiles[R_FLOWER_3].rc, &_player->getSearchRc()))
		{
			cout << "꽃3!" << endl;
		}
		if (IntersectRectToRect(&_tiles[BOOK].rc, &_player->getSearchRc()))
		{
			cout << "책!" << endl;
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
		sceneChange("garden_5f");
		cout << "정원으로!" << endl;
	}
	if (IntersectRectToRect(&_tiles[DOORTOPRISONWELL].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISONWELL + 1].rc, &_player->getPlayerFrc()))
	{
		sceneChange("prison_well_5f");
		cout << "우물로!" << endl;
	}
	if (IntersectRectToRect(&_tiles[DOORTOPRISONSKUL].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISONSKUL + TILEX].rc, &_player->getPlayerFrc()))
	{
		sceneChange("prison_skul_5f");
		cout << "해골방으로!" << endl;
	}
}

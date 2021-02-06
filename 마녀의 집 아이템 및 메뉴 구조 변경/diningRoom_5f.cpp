#include "stdafx.h"
#include "diningRoom_5f.h"
#include "Player.h"

HRESULT diningRoom_5f::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_player->setDirec(CHRDIREC_LEFT);

	//타일 불러오기
	load(_location);

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	fifthFloorStage::init();

	return S_OK;
}

void diningRoom_5f::release()
{
}

void diningRoom_5f::update()
{
	fifthFloorStage::update();

	setFrameIndex();

	//카메라 관련 업데이트
	cameraUpdate();
	setTrigger();
	//cout << "x : " << (int)(_player->getPlayerLocX()) / TILESIZE << " y : " << (int)(_player->getPlayerLocY()) / TILESIZE << endl;
}

void diningRoom_5f::render()
{	
	//배경 랜더
	IMAGEMANAGER->FindImage("배경62")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경62"),
		Vector2(IMAGEMANAGER->FindImage("배경62")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("배경62")->GetSize().y / 2));

	fifthFloorStage::render();

}

void diningRoom_5f::Collision()
{
}

void diningRoom_5f::load(LOCATION _location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/5f_diningRoom.map", GENERIC_READ, NULL, NULL,
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
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}
		break;
	}
	CloseHandle(file);
}

void diningRoom_5f::setTrigger()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// 클릭행동 트리거
	{
		if (IntersectRectToRect(&_tiles[Y_FLOWER_1].rc, &_player->getSearchRc()))
		{
			cout << "꽃1!" << endl;
		}
		if (IntersectRectToRect(&_tiles[Y_FLOWER_2].rc, &_player->getSearchRc()))
		{
			cout << "꽃2!" << endl;
		}
		if (IntersectRectToRect(&_tiles[Y_FLOWER_3].rc, &_player->getSearchRc()))
		{
			cout << "꽃3!" << endl;
		}
		if (IntersectRectToRect(&_tiles[Y_FLOWER_4].rc, &_player->getSearchRc()))
		{
			cout << "꽃4!" << endl;
		}
		if (IntersectRectToRect(&_tiles[CLOCK].rc, &_player->getSearchRc()))
		{
			cout << "시계!" << endl;
		}
		if (IntersectRectToRect(&_tiles[LIGHT].rc, &_player->getSearchRc()))
		{
			cout << "전등!" << endl;
		}
		if (IntersectRectToRect(&_tiles[DOORTODININGROOMITEM].rc, &_player->getPlayerFrc()))
		{
			cout << "아이템획득!!" << endl;
		}
	}

	if (IntersectRectToRect(&_tiles[DOORTOGARDEN].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOGARDEN + TILEX].rc, &_player->getPlayerFrc()))
	{
		_isChangeScene = true;
		sceneChange("garden_5f", CHRDIREC_RIGHT, LOCATION_1);
		cout << "정원으로!" << endl;
	}
	if (IntersectRectToRect(&_tiles[DROPFLOWER].rc, &_player->getPlayerFrc()))
	{
		_vFrameTile[2].isTrigger = true;
		cout << "화분!" << endl;
	}
}

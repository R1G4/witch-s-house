#include "stdafx.h"
#include "hallway.h"
#include "Player.h"


hallway::hallway()
{
}


hallway::~hallway()
{
}

HRESULT hallway::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_player->setDirec(_chrdirection);

	//타일 불러오기
	load(_location);

	//테스트로 넣어둠 원래 곰돌이를 넣었을때 초기화 해야함 추후에 지울것!
	//STAGEMEMORYMANAGER->setIsBearPut(true);
	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());

	firstFloorStage::init();

	_trigger = NONE;

	return S_OK;
}

void hallway::release()
{
	firstFloorStage::release();
}

void hallway::update()
{
	//프레임 인덱스 셋팅
	setFrameIndex();

	switch (_trigger)
	{
		case hallway::NONE:
			firstFloorStage::update();
			firstFloorStage::setAlpha();
			Collision();
			break;
		case hallway::DOOR_LEFT_OPEN:
			firstFloorStage::sceneChange("entrance", CHRDIREC_LEFT, LOCATION_1);
			break;
		case hallway::DOOR_RIGHT_TOP_OPEN:
			firstFloorStage::sceneChange("boxRoom", CHRDIREC_RIGHT, LOCATION_DEFAULT);
			break;
		case hallway::DOOR_RIGHT_BOTTOM_OPEN:
			firstFloorStage::sceneChange("bearRoom", CHRDIREC_RIGHT, LOCATION_DEFAULT);
			break;
		case hallway::CLOCK:
			firstFloorStage::setAlpha();
			_vScript = TEXTMANAGER->loadFile("dialog/1f/1f_hallwayClock.txt");

			if (KEYMANAGER->isOnceKeyUp(VK_SPACE) || KEYMANAGER->isOnceKeyUp('x'))
			{
				TEXTMANAGER->clearScript();
				_trigger = NONE;
				_vScript.clear();
			}
			break;
		default:
			_trigger = NONE;
			firstFloorStage::update();
			firstFloorStage::setAlpha();
			Collision();
			break;
	}	
	//카메라 관련 업데이트
	firstFloorStage::cameraUpdate();
}

void hallway::render()
{
	//배경 랜더
	IMAGEMANAGER->FindImage("배경11")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경11"),
		Vector2(IMAGEMANAGER->FindImage("배경11")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("배경11")->GetSize().y / 2));

	firstFloorStage::render();
}

void hallway::Collision()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			int index = i * TILEX + j;
			//어느 타일과 충돌 했을 경우
			if (!IntersectRectToRect(&_tiles[index].rc, &_player->getPlayerFrc())) continue;

			//타일 충돌(이동을 못하는 타일)은 같으므로 참조된 클래스에서 돌린다.
			firstFloorStage::tileCollision(i, j);

			//해당 타일의 속에 따라
			//추후에 타일 속성 예외가 적을 경우 스위치문에서 if문으로 변경 할 생각임
			switch (_tiles[index].terrain)
			{
			case TR_TRIGGER:
				//밞았을때
				//트리거 받아오기
				_trigger = index == 1036 ? DOOR_LEFT_OPEN :
						   index == DOOR_LEFT_OPEN ? DOOR_LEFT_OPEN :
					       index == 621 ? DOOR_RIGHT_TOP_OPEN :
						   index == DOOR_RIGHT_TOP_OPEN ? DOOR_RIGHT_TOP_OPEN :
						   index == 1041 ? DOOR_RIGHT_BOTTOM_OPEN:
						   index == DOOR_RIGHT_BOTTOM_OPEN ? DOOR_RIGHT_BOTTOM_OPEN : NONE;

				if (_trigger == DOOR_RIGHT_TOP_OPEN || _trigger == DOOR_RIGHT_BOTTOM_OPEN)
					autoSound("openDoarLong");
				else if(_trigger == DOOR_LEFT_OPEN)
					autoSound("openDoarShort");

				if (((TRIGGER)index == CLOCK  || (TRIGGER)index == 499) && KEYMANAGER->isOnceKeyUp(VK_SPACE))
					_trigger = CLOCK;
			}
		}
	}
}

void hallway::load(LOCATION location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/1f_hallway.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute != PLAYER)
			continue;

		//초기 위치를 잡아준다.
		switch (location)
		{
		case LOCATION_1:
			_player->setStart(560 % TILEX, 560 / TILEX);
			break;
		case LOCATION_2:
			_player->setStart(980 % TILEX, 980 / TILEX);
			break;
		case LOCATION_DEFAULT: default:
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}
	}
	CloseHandle(file);
}
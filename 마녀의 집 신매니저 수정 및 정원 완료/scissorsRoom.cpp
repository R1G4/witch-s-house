#include "stdafx.h"
#include "scissorsRoom.h"
#include "Player.h"

scissorsRoom::scissorsRoom()
{
}

scissorsRoom::~scissorsRoom()
{
}

HRESULT scissorsRoom::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_player->setDirec(_chrdirection);

	//타일 불러오기
	load();

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	firstFloorStage::init();

	_trigger = NONE;

	return S_OK;
}

void scissorsRoom::release()
{
	firstFloorStage::release();
}

void scissorsRoom::update()
{
	//프레임 인덱스 셋팅
	setFrameIndex();

	switch (_trigger)
	{
	case scissorsRoom::NONE:
		firstFloorStage::update();
		Collision();
		break;
	case scissorsRoom::DOOR_RIGHT_OPEN:
		firstFloorStage::sceneChange("entrance", CHRDIREC_RIGHT, LOCATION_2);
		break;

	default:
		_trigger = NONE;
		firstFloorStage::update();
		Collision();
		break;
	}

	firstFloorStage::cameraUpdate();
}

void scissorsRoom::render()
{
	//배경 랜더
	IMAGEMANAGER->FindImage("배경14")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경14"),
		Vector2(IMAGEMANAGER->FindImage("배경14")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("배경14")->GetSize().y / 2));

	firstFloorStage::render();
}

void scissorsRoom::Collision()
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
			cout << "x: " << i << "  y: " << j << "  index: " << index << endl;

			switch (_tiles[index].terrain)
			{
			case TR_TRIGGER:
				cout << "x: " << i << "  y: " << j << "  index: " << index << endl;
				//트리거 받아오기
				_trigger = index == 564 ? DOOR_RIGHT_OPEN : (TRIGGER)index;

				break;
			}
		}
	}
}

void scissorsRoom::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/scissorsRoom.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute == PLAYER)
		{
			cout << i % TILEX << "&&, " << i / TILEX << endl;
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}
	}
	CloseHandle(file);
}

void scissorsRoom::getMemory()
{
	for (int k = 0; k < _vFrameTile.size(); k++)
	{
		if (!STAGEMEMORYMANAGER->getIsCandle())
			continue;

		if (_vFrameTile[k].keyName == "가위")
		{
			//트리거가 이미 발동되었던 상태로 셋팅한다.
			_vFrameTile[k].isMaxframe = true;
		}
		else if (_vFrameTile[k].keyName == "곰")
		{
			//에너미 트리거 발동
		}
	}
}
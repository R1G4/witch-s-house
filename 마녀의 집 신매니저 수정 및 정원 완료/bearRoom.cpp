#include "stdafx.h"
#include "bearRoom.h"
#include "Player.h"


bearRoom::bearRoom()
{
}


bearRoom::~bearRoom()
{
}

HRESULT bearRoom::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_player->setDirec(_chrdirection);

	//타일 불러오기
	load();

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	firstFloorStage::init();

	_trigger = NONE;

	return S_OK;
}

void bearRoom::release()
{
	firstFloorStage::release();
}

void bearRoom::update()
{
	//프레임 인덱스 셋팅
	setFrameIndex();

	switch (_trigger)
	{
	case bearRoom::NONE:
		firstFloorStage::update();
		Collision();
		break;
	case bearRoom::DOOR_LEFT_OPEN:
		firstFloorStage::sceneChange("hallway", CHRDIREC_LEFT, LOCATION_2);
		break;

	default:
		_trigger = NONE;
		firstFloorStage::update();
		Collision();
			break;
	}

	firstFloorStage::cameraUpdate();
}

void bearRoom::render()
{
	//배경 랜더
	IMAGEMANAGER->FindImage("배경12")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경12"),
		Vector2(IMAGEMANAGER->FindImage("배경12")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("배경12")->GetSize().y / 2));

	firstFloorStage::render();
}

void bearRoom::Collision()
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
				_trigger = index == 556 ? DOOR_LEFT_OPEN : (TRIGGER)index;

				break;
			}
		}
	}
}

void bearRoom::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/bearRoom.map", GENERIC_READ, NULL, NULL,
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
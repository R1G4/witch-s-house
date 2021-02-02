#include "stdafx.h"
#include "entranceFake.h"
#include "player.h"


entranceFake::entranceFake()
{
}


entranceFake::~entranceFake()
{
}

HRESULT entranceFake::init()
{
	_player->setDirec(CHRDIREC_UP);

	//타일 불러오기
	load();

	firstFloorStage::init();

	return S_OK;
}

void entranceFake::release()
{
	firstFloorStage::release();
}

void entranceFake::update()
{
	//프레임 인덱스 셋팅
	setFrameIndex();

	switch (_trigger)
	{
	case entranceFake::DOOR_CLOSE: case entranceFake::NONE:
		//문이 닫힌 상태라면 충돌 및 플레이어 무브 가능하게 한다.
		firstFloorStage::update();
		//충돌처리
		Collision();
		break;
	case entranceFake::DOOR_OPEN:
		//문이 열린 상태라면 화면 투명도 조절하여 일정 투명도에 도달 할 경우 씬 전환을 한다.
		firstFloorStage::sceneChange("entranceTrap");
		break;
	}

	//카메라 관련 업데이트
	firstFloorStage::cameraUpdate();
}

void entranceFake::render()
{
	//배경 랜더
	IMAGEMANAGER->FindImage("배경6")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경6"),
		Vector2(IMAGEMANAGER->FindImage("배경6")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("배경6")->GetSize().y / 2));

	firstFloorStage::render();
}

void entranceFake::Collision()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			int index = i * TILEX + j;

			//어느 타일과 충돌하지 못한 경우 무시
			if (!IntersectRectToRect(&_tiles[index].rc, &_player->getPlayerFrc())) continue;

			cout << "x: " << j << "  y: " << i << "  index: " << index << endl;

			//타일 충돌(이동을 못하는 타일)은 같으므로 참조된 클래스에서 돌린다.
			firstFloorStage::tileCollision(i, j);

			//해당 타일의 속에 따라
			switch (_tiles[index].terrain)
			{
			case TR_TRIGGER:
				//만약 타일의 프레임 이미지가 문.. 머시기라면
				if (_tiles[index].keyName.find("문") != string::npos && _tiles[index].keyName.size() <= 3)
				{
					//해당 프레임 이미지를 찾아서
					for (int k = 0; k < _vFrameTile.size(); k++)
					{
						if (_vFrameTile[k].indexX == i && _vFrameTile[k].indexY == j && _vFrameTile[k].keyName == _tiles[index].keyName)
						{
							//트리거를 발동한다.
							_vFrameTile[k].isTrigger = true;
							_trigger = DOOR_OPEN;
						}
					}
				}
				break;
			}
		}
	}
}

void entranceFake::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/entranceFake.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute == PLAYER)
		{
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}
	}
	CloseHandle(file);
}
#include "stdafx.h"
#include "hallway.h"
#include "player.h"

hallway::hallway()
{
}


hallway::~hallway()
{
}

HRESULT hallway::init()
{
	_player->setDirec(CHRDIREC_RIGHT);

	//타일 불러오기
	load();

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	firstFloorStage::init();

	return S_OK;
}

void hallway::release()
{
	firstFloorStage::release();
}

void hallway::update()
{	
	//프레임 인덱스 셋팅
	setFrameIndex();;

	//switch (_trigger)
	//{
	//case hallway::DOOR_CLOSE: case hallway::NONE:
	//	//문이 닫힌 상태라면 충돌 및 플레이어 무브 가능하게 한다.
	//	firstFloorStage::update();
	//	Collision();
	//	break;
	//case hallway::DOOR_OPEN:
	//	//문이 열린 상태라면 화면 투명도 조절하여 일정 투명도에 도달 할 경우 씬 전환을 한다.
	//	firstFloorStage::sceneChange("entrance");
	//	break;
	//case hallway::READ:
	//	//추가예정
	//	//샬라샬라~ 여기서 다이얼로그 보여줘야함
	//	//읽는 미션 성공
	//	_mission.read = SUCCESS;
	//	_trigger = NONE;
	//	break;
	//}

	//카메라 관련 업데이트
	firstFloorStage::cameraUpdate();
}

void hallway::render()
{
	//배경 랜더
	IMAGEMANAGER->FindImage("배경7")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경7"),
		Vector2(IMAGEMANAGER->FindImage("배경7")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("배경7")->GetSize().y / 2));

	firstFloorStage::render();
}

void hallway::Collision()
{
	//for (int i = 0; i < TILEY; i++)
	//{
	//	for (int j = 0; j < TILEX; j++)
	//	{
	//		int index = i * TILEX + j;

	//		//어느 타일과 충돌 했을 경우
	//		if (!IntersectRectToRect(&_tiles[index].rc, &_player->getPlayerFrc())) continue;
	//		//타일 충돌(이동을 못하는 타일)은 같으므로 참조된 클래스에서 돌린다.
	//		firstFloorStage::tileCollision(i, j);
	//		//해당 타일의 속성에 따라
	//		//추후에 타일 속성 예외가 적을 경우 스위치문에서 if문으로 변경 할 생각임
	//		switch (_tiles[index].terrain)
	//		{
	//		case TR_TRIGGER:
	//			//트리거를 받아온다
	//			_trigger = index == DOOR_OPEN ? (_mission.read ? DOOR_OPEN : _trigger) :
	//				index == READ ? (!_mission.read ? READ : _trigger) : _trigger;

	//			//트리거와 프레임 이미지가 같이 위치한 경우
	//			//해당 프레임 이미지를 찾아서
	//			for (int k = 0; k < _vFrameTile.size(); k++)
	//			{
	//				if (_vFrameTile[k].indexX == i && _vFrameTile[k].indexY == j && _vFrameTile[k].keyName == _tiles[index].keyName)
	//				{
	//					//트리거를 발동한다.
	//					_vFrameTile[k].isTrigger = true;
	//				}
	//			}
	//			break;
	//		}

	//	}
	//}
}

void hallway::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/hallway.map", GENERIC_READ, NULL, NULL,
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
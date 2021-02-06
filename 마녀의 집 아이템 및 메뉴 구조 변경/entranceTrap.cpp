#include "stdafx.h"
#include "entranceTrap.h"
#include "player.h"


entranceTrap::entranceTrap()
{
}


entranceTrap::~entranceTrap()
{
}

HRESULT entranceTrap::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_player->setDirec(_chrdirection);

	//타일 불러오기
	load();

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	firstFloorStage::init();

	_trigger = NONE;

	return S_OK;
}

void entranceTrap::release()
{
	firstFloorStage::release();
}

void entranceTrap::update()
{	
	//프레임 인덱스 셋팅
	setFrameIndex();;

	switch (_trigger)
	{
	case entranceTrap::NONE:
		//문이 닫힌 상태라면 충돌 및 플레이어 무브 가능하게 한다.
		firstFloorStage::update();
		firstFloorStage::setAlpha();
		Collision();
		break;
	case entranceTrap::DOOR_OPEN:
		//문이 열린 상태라면 화면 투명도 조절하여 일정 투명도에 도달 할 경우 씬 전환을 한다.
		firstFloorStage::sceneChange("entrance", CHRDIREC_DOWN, LOCATION_DEFAULT);
		break;
	case entranceTrap::READ:
		firstFloorStage::setAlpha();
		//추가예정
		//샬라샬라~ 여기서 다이얼로그 보여줘야함
		//읽는 미션 성공
		_mission.read = SUCCESS;
		_trigger = NONE;
		break;
	default:
		_trigger = NONE;
		firstFloorStage::setAlpha();
		firstFloorStage::update();
		Collision();
		break;
	}
	
	//카메라 관련 업데이트
	firstFloorStage::cameraUpdate();
}

void entranceTrap::render()
{
	//배경 랜더
	IMAGEMANAGER->FindImage("배경7")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경7"),
		Vector2(IMAGEMANAGER->FindImage("배경7")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("배경7")->GetSize().y / 2));

	firstFloorStage::render();
}

void entranceTrap::Collision()
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
			//해당 타일의 속성에 따라
			//추후에 타일 속성 예외가 적을 경우 스위치문에서 if문으로 변경 할 생각임
			switch (_tiles[index].terrain)
			{
			case TR_TRIGGER:
				//트리거를 받아온다
				_trigger = index == DOOR_OPEN ? (_mission.read ? DOOR_OPEN : _trigger) :
					index == READ ? (!_mission.read ? READ : _trigger) : _trigger;

				//트리거와 프레임 이미지가 같이 위치한 경우
				//해당 프레임 이미지를 찾아서
				for (int k = 0; k < _vFrameTile.size(); k++)
				{
					if (_vFrameTile[k].indexX == i && _vFrameTile[k].indexY == j && _vFrameTile[k].keyName == _tiles[index].keyName)
					{
						//트리거를 발동한다.
						_vFrameTile[k].isTrigger = true;
					}
				}
				break;
			}

		}
	}
}

void entranceTrap::load(LOCATION location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/1f_entranceTrap.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute != PLAYER)
			continue;

		if (location)	_player->setStart(i%TILEX, i / TILEX);
		break;
	}
	CloseHandle(file);
}
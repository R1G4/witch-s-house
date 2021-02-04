#include "stdafx.h"
#include "entrance.h"
#include "Player.h"


entrance::entrance()
{
}


entrance::~entrance()
{
}

HRESULT entrance::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_player->setDirec(_chrdirection);

	//타일 불러오기
	load(_location);

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	firstFloorStage::init();

	_trigger = NONE;
	getMemory();
	setFrameIndex();
	return S_OK;
}

void entrance::release()
{
	firstFloorStage::release();
}

void entrance::getMemory()
{
	for (int k = 0; k < _vFrameTile.size(); k++)
	{
		if (!STAGEMEMORYMANAGER->getIsCandle())
			continue;

		if (_vFrameTile[k].keyName == "켜진초꺼진초")
		{
			//트리거가 이미 발동되었던 상태로 셋팅한다.
			_vFrameTile[k].isMaxframe = true;
		}
		else if (_vFrameTile[k].keyName == "꽃병프레임")
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

void entrance::update()
{
	//프레임 인덱스 셋팅
	setFrameIndex();
	switch (_trigger)
	{
	case entrance::NONE:
		_delay = 0;
		firstFloorStage::update();
		Collision();
		break;
	case entrance::DOOR_LEFT_OPEN:
		firstFloorStage::sceneChange("scissorsRoom", CHRDIREC_LEFT, LOCATION_DEFAULT);
		break;
	case entrance::DOOR_RIGHT_OPEN:	
		firstFloorStage::sceneChange("hallway", CHRDIREC_RIGHT, LOCATION_DEFAULT);
		break;
	case entrance::CAT_TALK:
		break;
	case entrance::CANDLE_OFF:
		STAGEMEMORYMANAGER->setIsCandle(true);
		_trigger = DELAY;
		break;
	case entrance::VASE_DOWN:
		STAGEMEMORYMANAGER->setIsVase(true);
		_trigger = DELAY;
		break;
	case entrance::DELAY:
		_player->setState(CHR_IDLE);
		_delay++;
		if (_delay % 70 == 0)
			_trigger = NONE;
		break;
	default:
		_trigger = NONE;
		firstFloorStage::update();
		Collision();
		break;
	}	
	
	//카메라 관련 업데이트
	firstFloorStage::cameraUpdate();
}

void entrance::render()
{
	//배경 랜더
	IMAGEMANAGER->FindImage("배경8")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경8"),
		Vector2(IMAGEMANAGER->FindImage("배경8")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("배경8")->GetSize().y / 2));

	firstFloorStage::render();
}

void entrance::Collision()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			int index = i * TILEX + j;
			//어느 타일과 충돌 했을 경우
			if (!IntersectRectToRect(&_tiles[index].rc, &_player->getPlayerFrc())) continue;
			cout << "x: " << j << "  y: " << i << "  index: " << index << endl;
			//타일 충돌(이동을 못하는 타일)은 같으므로 참조된 클래스에서 돌린다.
			firstFloorStage::tileCollision(i, j);
			//해당 타일의 속에 따라
			//추후에 타일 속성 예외가 적을 경우 스위치문에서 if문으로 변경 할 생각임
			switch (_tiles[index].terrain)
			{
			case TR_TRIGGER:
				
				if ((TRIGGER)index != CANDLE_OFF && (TRIGGER)index != VASE_DOWN)
				{
					_trigger = index == 556 ? DOOR_LEFT_OPEN : 
							   index == 568 ? DOOR_RIGHT_OPEN : (TRIGGER)index;
					break;
				}
				else
				{
					for (int k = 0; k < _vFrameTile.size(); k++)
					{
						if (_vFrameTile[k].isMaxframe || _vFrameTile[k].isTrigger)
							continue;

						if ((TRIGGER)index == CANDLE_OFF && _vFrameTile[k].keyName == "켜진초꺼진초")
						{
							//트리거를 발동한다.
							_vFrameTile[k].isTrigger = true;
							_trigger = CANDLE_OFF;
						}
						else if ((TRIGGER)index == VASE_DOWN && _vFrameTile[k].keyName == "꽃병프레임")
						{
							//트리거를 발동한다.
							_vFrameTile[k].isTrigger = true;
							_trigger = VASE_DOWN;
						}
					}
				}
				break;
			}
		}
	}
}

void entrance::load(LOCATION location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/entrance.map", GENERIC_READ, NULL, NULL,
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
			_player->setStart(507 % TILEX, 507 / TILEX);
			break;
		case LOCATION_2:
			_player->setStart(498 % TILEX, 498 / TILEX);
			break;
		case LOCATION_DEFAULT: default:
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}
		break;
	}
	CloseHandle(file);
}
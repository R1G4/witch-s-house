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
	//플레이어가 바라보는 방향
	_player->setDirec(_chrdirection);

	//타일 불러오기
	load(_location);

	//카메라 셋팅
	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());

	//1층 관련 스테이지 초기화
	firstFloorStage::init();

	//초기 트리거 상태
	_trigger = NONE;

	//에너미 등장 인터벌
	_enemyInterval = 0;

	//스테이지 메모리 불러오기
	getMemory();

	//프레임 이미지 프레임 셋팅
	setFrameIndex();

	return S_OK;
}

void entrance::bearCom()
{
	if (!STAGEMEMORYMANAGER->getIsBearComing() ||
		STAGEMEMORYMANAGER->getIsBearComing2() ||
		!STAGEMEMORYMANAGER->getIsBearPickUp() ||
		!STAGEMEMORYMANAGER->getIsBearPut())
		return;

	_enemyInterval++;
	if (_enemyInterval == 80)
	{
		_bear = new bear;
		_bear->init(496 % TILEX, 496 / TILEX);
		_playerTile = new astarTile;
		_enemyTile = new astarTile;
		_dead = new DeadManager;
		_dead->init();
		_dead->setPlayerAddress(_player);
		for (int k = 0; k < TILEX*TILEY; k++)
			_objTile[k] = new astarTile;
		bossLocX = 496 % TILEX;
		bossLocY = 496 / TILEX;

		firstFloorStage::objectLocation();

		STAGEMEMORYMANAGER->setIsBearComing2(true);
	}
}

void entrance::release()
{
	firstFloorStage::release();
}

void entrance::getMemory()
{
	for (int k = 0; k < _vFrameTile.size(); k++)
	{
		if (_vFrameTile[k].keyName == "켜진초꺼진초" && STAGEMEMORYMANAGER->getIsCandle())
		{
			//트리거가 이미 발동되었던 상태로 셋팅한다.
			_vFrameTile[k].isMaxframe = true;
		}
		else if (_vFrameTile[k].keyName == "꽃병프레임" && STAGEMEMORYMANAGER->getIsVase())
		{
			//트리거가 이미 발동되었던 상태로 셋팅한다.
			_vFrameTile[k].isMaxframe = true;
		}
		else if (_vFrameTile[k].keyName == "손바닥" && (STAGEMEMORYMANAGER->getIsPalmLeft() || (STAGEMEMORYMANAGER->getIsPalmRight())))
		{
			//트리거가 이미 발동되었던 상태로 셋팅한다.
			_vFrameTile[k].isMaxframe = true;
		}
	}
}

void entrance::update()
{
	bearCom();

	//프레임 인덱스 셋팅
	setFrameIndex();
	switch (_trigger)
	{
	case entrance::NONE:
		_delay = 0;
		firstFloorStage::update();
		firstFloorStage::setAlpha();
		Collision();
		break;
	case entrance::DOOR_LEFT_OPEN:
		firstFloorStage::sceneChange("scissorsRoom", CHRDIREC_LEFT, LOCATION_DEFAULT);
		break;
	case entrance::DOOR_RIGHT_OPEN:	
		firstFloorStage::sceneChange("hallway", CHRDIREC_RIGHT, LOCATION_DEFAULT);
		break;
	case entrance::CAT_TALK:
		firstFloorStage::setAlpha();
		break;
	case entrance::CANDLE_OFF:
		firstFloorStage::setAlpha();
		STAGEMEMORYMANAGER->setIsCandle(true);
		_trigger = DELAY;
		break;
	case entrance::VASE_DOWN:
		firstFloorStage::setAlpha();
		STAGEMEMORYMANAGER->setIsVase(true);
		_trigger = DELAY;
		break;
	case entrance::PALM_LEFT:
		firstFloorStage::setAlpha();
		_isBlood = true;
		_trigger = DELAY;
		break;
	case entrance::PALM_RIGHT:
		firstFloorStage::setAlpha();
		_isBlood = true;
		_trigger = DELAY;
		break;
	case entrance::DELAY:
		firstFloorStage::setAlpha();
		if(_bear) _player->setState(CHR_IDLE);
		_delay++;
		if (_delay % 60 == 0)
			_trigger = NONE;
		break;
	default:
		_trigger = NONE;
		firstFloorStage::update();
		Collision();
		break;
	}	

	//카메라 관련 업데이트
	if (_bear)	firstFloorStage::enemyUpdate();

	if(!_bear && _dead) _dead->update();
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

			//어느 타일과 플레이어 상호작용 렉트가 충돌하였다면
			if (IntersectRectToRect(&_tiles[index].rc, &_player->getSearchRc()))
			{
				//텍스를 넣는 동시에 폼 실행
				if ((TRIGGER)index == CAT_TALK)
				{
					STORAGEMANAGER->loadView();

					//_trigger = CAT_TALK;
				}
			}


			//어느 타일과 충돌 했을 경우
			if (!IntersectRectToRect(&_tiles[index].rc, &_player->getPlayerFrc())) continue;


			firstFloorStage::tileCollision(i, j);
			//해당 타일의 속에 따라
			//추후에 타일 속성 예외가 적을 경우 스위치문에서 if문으로 변경 할 생각임
			switch (_tiles[index].terrain)
			{
			case TR_TRIGGER:
				
				if ((TRIGGER)index == PALM_LEFT || (TRIGGER)index == PALM_RIGHT)
				{
					if (_isBlood && STAGEMEMORYMANAGER->getIsScissors())
					{
						for (int k = 0; k < _vFrameTile.size(); k++)
						{
							if (_vFrameTile[k].isMaxframe || _vFrameTile[k].isTrigger)
								continue;

							if ((TRIGGER)index == PALM_LEFT && _vFrameTile[k].keyName == "손바닥"
								&& ((_vFrameTile[k].indexX == 5 && _vFrameTile[k].indexY == 18) || (_vFrameTile[k].indexX == 7 && _vFrameTile[k].indexY == 17)))
							{
								//트리거를 발동한다.
								_vFrameTile[k].isTrigger = true;
								_trigger = (TRIGGER)index;
								STAGEMEMORYMANAGER->setIsPalmLeft(true);
							}
							else if ((TRIGGER)index == PALM_RIGHT && _vFrameTile[k].keyName == "손바닥" && _vFrameTile[k].indexX == 7 && _vFrameTile[k].indexY == 27)
							{
								//트리거를 발동한다.
								_vFrameTile[k].isTrigger = true;
								_trigger = (TRIGGER)index;
								//STAGEMEMORYMANAGER->setIsPalmRight(true);
								STAGEMEMORYMANAGER->setIsPalmRight(true);
							}
						}
					}//곰돌이 가져간 상태라면 (미구현 상태이므로 true로 해둠)
					else if(!_isBlood/*STAGEMEMORYMANAGER->getIsBearPickUp()*/)
						_trigger = (TRIGGER)index;
				}
				else if ((TRIGGER)index != CANDLE_OFF && (TRIGGER)index != VASE_DOWN && (TRIGGER)index != CAT_TALK)
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
						//곰돌이를 바구니에 넣은 상태
						else if (STAGEMEMORYMANAGER->getIsBearPut() && (TRIGGER)index == VASE_DOWN && _vFrameTile[k].keyName == "꽃병프레임")
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
	file = CreateFile("Stage/1f_entrance.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		switch (_tiles[i].attribute)
		{
			case PLAYER:
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
	}
	CloseHandle(file);
}
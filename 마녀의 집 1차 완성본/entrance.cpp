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
	//곰 생성 조건
	//에너미 곰1 이 등장한 적이 없거나 에너미 곰2 이 등장한적 없거나 곰돌이 미션을 전부 실패하지 않은 경우를 제외한다.
	if (!STAGEMEMORYMANAGER->getIsBearComing() ||
		STAGEMEMORYMANAGER->getIsBearComing2() ||
		!STAGEMEMORYMANAGER->getIsBearPickUp() ||
		!STAGEMEMORYMANAGER->getIsBearPut())
		return;

	//바로 등장하지 않고 이벤트 발생 및 뜸을 들인 후
	_enemyInterval++;
	if (_enemyInterval == 1)
		autoSound("호러3");

	//해당 간격이 도달 했을때 생성한다.
	if (_enemyInterval == 88)
	{
		autoSound("곰등장");
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
		//타일에 켜진초꺼진초가 존재하며 메모리에 트리거가 이미 발생된 적이 있다면
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

	//트리거 상태에 따른 호출 및 설정
	switch (_trigger)
	{
	case entrance::NONE:
		_delay = 0;
		firstFloorStage::update();
		firstFloorStage::setAlpha();
		Collision();
		break;
	case entrance::DOOR_LEFT_OPEN:
		//가위방을 이동
		firstFloorStage::sceneChange("scissorsRoom", CHRDIREC_LEFT, LOCATION_DEFAULT);
		break;
	case entrance::DOOR_RIGHT_OPEN:
		//복도로 이동
		firstFloorStage::sceneChange("hallway", CHRDIREC_RIGHT, LOCATION_DEFAULT);
		break;
	case entrance::CAT_TALK:
		//고양이와 대화
		firstFloorStage::setAlpha();
		break;
	case entrance::CANDLE_OFF:
		//촛불이 꺼진 상태
		firstFloorStage::setAlpha();

		//메모리에 저장한다.
		STAGEMEMORYMANAGER->setIsCandle(true);

		//트리거가 발생되었으므로 멈칫하는 트리거로 설정한다.
		_trigger = DELAY;
		break;
	case entrance::VASE_DOWN:
		//화분이 떨어진 상태
		firstFloorStage::setAlpha();

		//메모리에 저장한다.
		STAGEMEMORYMANAGER->setIsVase(true);

		//트리거가 발생되었으므로 멈칫하는 트리거로 설정한다.
		_trigger = DELAY;
		break;
	case entrance::PALM_LEFT:
		//왼쪽 손자국을 찍는 상태
		firstFloorStage::setAlpha();

		//기존에는 랜더하지 않다가 _isBlood true로 하면서 랜더한다.
		_isBlood = true;

		//트리거가 발생되었으므로 멈칫하는 트리거로 설정한다.
		_trigger = DELAY;
		break;
	case entrance::PALM_RIGHT:
		//오른쪽 손자국을 찍는 상태
		firstFloorStage::setAlpha();

		//기존에는 랜더하지 않다가 _isBlood true로 하면서 랜더한다.
		_isBlood = true;

		//트리거가 발생되었으므로 멈칫하는 트리거로 설정한다.
		_trigger = DELAY;
		break;
	case entrance::DELAY:
		firstFloorStage::setAlpha();

		//실제 게임에서 트리거 발동 시 멈칫 멈칫 하는걸 구현한거
		if (_bear) _player->setState(CHR_IDLE);
		_delay++;
		if (_delay % 60 == 0)
			_trigger = NONE;
		break;
	default:
		//그외의 상태는 NONE과 같다.
		_trigger = NONE;
		firstFloorStage::update();
		Collision();
		break;
	}

	//에너미 곰이 존재 한다면 해당 함수를 호출한다.
	if (_bear)	firstFloorStage::enemyUpdate();
	if (_dead && _dead->getIsDead())
	{
		SAFE_RELEASE(_bear);
		SAFE_DELETE(_bear);
		_dead->update();
	}
	//에너미 곰이 존재 하지 않으며 데드씬이 존재한다면 데드씬 함수를 호출한다.
	//if(!_bear && _dead) _dead->update();

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
								autoSound("피2");
								//트리거를 발동한다.
								_vFrameTile[k].isTrigger = true;
								_trigger = (TRIGGER)index;
								STAGEMEMORYMANAGER->setIsPalmLeft(true);
							}
							else if ((TRIGGER)index == PALM_RIGHT && _vFrameTile[k].keyName == "손바닥" && _vFrameTile[k].indexX == 7 && _vFrameTile[k].indexY == 27)
							{
								autoSound("피1");
								//트리거를 발동한다.
								_vFrameTile[k].isTrigger = true;
								_trigger = (TRIGGER)index;
								//STAGEMEMORYMANAGER->setIsPalmRight(true);
								STAGEMEMORYMANAGER->setIsPalmRight(true);
							}
						}
					}//곰돌이 가져간 상태라면 (미구현 상태이므로 true로 해둠)
					else if (!_isBlood/*STAGEMEMORYMANAGER->getIsBearPickUp()*/)
						_trigger = (TRIGGER)index;
				}
				else if ((TRIGGER)index != CANDLE_OFF && (TRIGGER)index != VASE_DOWN && (TRIGGER)index != CAT_TALK)
				{
					autoSound("openDoarShort");
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
							autoSound("호로1");
							//트리거를 발동한다.
							_vFrameTile[k].isTrigger = true;
							_trigger = CANDLE_OFF;
						}
						//곰돌이를 바구니에 넣은 상태
						else if (STAGEMEMORYMANAGER->getIsBearPut() && (TRIGGER)index == VASE_DOWN && _vFrameTile[k].keyName == "꽃병프레임")
						{
							//사운드 추가 쨍그랑
							autoSound("깨짐1");
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
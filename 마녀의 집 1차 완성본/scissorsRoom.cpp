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
	lastEvent = false;

	//플레이어가 바라보는 방향
	_player->setDirec(_chrdirection);

	//타일 불러오기
	load();

	//카메라 셋팅
	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());

	//1층 관련 스테이지 초기화
	firstFloorStage::init();

	//초기 트리거 상태
	_trigger = NONE;

	//스테이지 메모리 불러오기
	getMemory();
	return S_OK;
}

void scissorsRoom::release()
{
	firstFloorStage::release();
}

void scissorsRoom::update()
{
	//
	//프레임 인덱스 셋팅
	setFrameIndex();
	switch (_trigger)
	{
	case scissorsRoom::NONE:
		_delay = 0;
		firstFloorStage::update();
		firstFloorStage::setAlpha();
		Collision();
		break;
	case scissorsRoom::DOOR_RIGHT_OPEN:
		firstFloorStage::sceneChange("entrance", CHRDIREC_RIGHT, LOCATION_2);
		break;
	case scissorsRoom::PALM:
		firstFloorStage::setAlpha();
		_isBlood = true;
		_trigger = DELAY;
		break;
	case scissorsRoom::SCISSORS:
 		STAGEMEMORYMANAGER->setIsScissors(true);
		//해당 프레임 이미지를 찾아서
		_player->setState(CHR_IDLE);
		_delay++;
		if (_delay % 110 == 0)	autoSound("노크");
		if (_delay % 150 == 0)	_vScript = TEXTMANAGER->loadFile("dialog/1f/1f_scissorsRoom.txt");
		if (_delay % 205 == 0)
		{
			if (_vScript.size() > 0) _vScript.clear();
			_trigger = NONE;
			_delay = 0;
			//해당 프레임 이미지를 찾아서
			for (int k = 0; k < _vFrameTile.size(); k++)
			{
				if (_vFrameTile[k].keyName.find("문") != string::npos && _vFrameTile[k].keyName.size() <= 3)
				{
					TEXTMANAGER->clearScript();
					//트리거를 발동한다.
					_vFrameTile[k].isTrigger = true;
					_vScript.clear();
					break;
				}
			}
		}
		break;	
	case scissorsRoom::BEARCOM:
		STAGEMEMORYMANAGER->setIsBearComing(true); 
		//STAGEMEMORYMANAGER->setIsBearComing2(true);
		_trigger = NONE;
		break;
	case scissorsRoom::DELAY:
		firstFloorStage::setAlpha();
		if (!_dead)
		{
			_player->setState(CHR_IDLE);
			_delay++;
			if (_delay % 60 == 0)
			{
				TEXTMANAGER->clearScript();
				if (_vScript.size() > 0) _vScript.clear();
				_trigger = NONE;
				_delay = 0;
			}
		}
		else _trigger = NONE;
		break;
	case scissorsRoom::DOOR_UP_OPEN:
		firstFloorStage::setAlpha();
		_isBlood = true;
		break;
	case scissorsRoom::DOOR_UP_OPEN2:
		lastEvent = true;
		if(_sceneAlpha > 0)
		{
			_sceneAlpha -= 0.008f;
			if (_sceneAlpha <= 0.65f)
				_sceneAlpha -= 0.021f;

			_player->setAlpha(_sceneAlpha);
		}
		if (_sceneAlpha <= 0.f)
		{
			_delay++;
			if(_delay > 140)
				SCENEMANAGER->changeScene("stairs_2F");
		}
		//firstFloorStage::sceneChange("stairs_2F");
		break;
	default:
		_delay= 0 ;
		_trigger = NONE;
		firstFloorStage::update();
		firstFloorStage::setAlpha();
		Collision();
		break;
	}
	if(_bear)	firstFloorStage::enemyUpdate();
	if (!_bear && _dead) _dead->update();
	firstFloorStage::cameraUpdate();
}

void scissorsRoom::render()
{
	//배경 랜더
	IMAGEMANAGER->FindImage("배경14")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경14"),
		Vector2(IMAGEMANAGER->FindImage("배경14")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("배경14")->GetSize().y / 2));

	
	//WINSIZEX / 2 - 250, WINSIZEY / 2 - 50
	if (lastEvent && _sceneAlpha < 0.1f)
	{
		////백그라운드 컬러 렉트
		//D2DINS->FillRectangle
		//(Vector2(_player->getPlayerLocX(), _player->getPlayerLocY()),
		//	Vector2(1920, 1280), 
		//	Pivot::LeftTop, 
		//	D2D1::ColorF::DarkGray, 1.f);
		IMAGEMANAGER->FindImage("Back2")->SetAlpha(0.7f);
		IMAGEMANAGER->FindImage("Back2")->SetSize(Vector2(1920, 1280));
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("Back2"), Vector2(_player->getPlayerLocX(), _player->getPlayerLocY()));
		IMAGEMANAGER->FindImage("곰들")->SetScale(1.1f);
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("곰들"), Vector2(_player->getPlayerLocX(), _player->getPlayerLocY()));
	}
	else
		firstFloorStage::render();
}

void scissorsRoom::Collision()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			int index = i * TILEX + j;

			//어느 타일과 플레이어 상호작용 렉트가 충돌하였다면
			if (IntersectRectToRect(&_tiles[index].rc, &_player->getSearchRc()))
			{
				//곰을 가져간 상태이며 가위를 자르지 않았고 바구니에 넣지도 않았을때만 
				if (!STAGEMEMORYMANAGER->getIsScissors() && 
					!STAGEMEMORYMANAGER->getIsBearPut() &&
					!STAGEMEMORYMANAGER->getIsBearComing() &&
					STAGEMEMORYMANAGER->getIsBearPickUp())

				//텍스를 넣는 동시에 폼 실행
				if ((TRIGGER)index == SCISSORS && SelectionForm(L"곰인형의 팔 다리를 자른다.", L"아무것도 하지 않는다.") && _fromSelected == LEFT)
				{
					//아이템 매니저 - 아이템 제거 후 추가
					//제거할 아이템의 key를 넣는다
					//제거 성공 할 경우
					if (ITEMMANAGER->removeItem("obj58"))
					{
						//곰의 몸톰을 추가한다.
						//추가를 성공 할 경우 트리거를 수정해준다.
						if (!ITEMMANAGER->addItem("obj11")) continue;

 						for (int k = 0; k < _vFrameTile.size(); k++)
						{
							//트리거가 이미 발동된 상태이거나 혹은 마지막 프레임만 보여준다면 무시한다.
							if (_vFrameTile[k].isMaxframe || _vFrameTile[k].isTrigger)
								continue;

							if (_vFrameTile[k].keyName == "가위")
							{
								//트리거를 발동한다.
								_vFrameTile[k].isTrigger = true;
								_trigger = SCISSORS;
								autoSound("곰짜를때");
								break;
							}
						}
					}
				}
			}

			//어느 타일과 충돌 했을 경우
			if (!IntersectRectToRect(&_tiles[index].rc, &_player->getPlayerFrc())) continue;

			//타일 충돌(이동을 못하는 타일)은 같으므로 참조된 클래스에서 돌린다.
			firstFloorStage::tileCollision(i, j);

			switch (_tiles[index].terrain)
			{
			case TR_TRIGGER:
				//트리거를 바로 받아와 본다.
				_trigger = index == 564 ? DOOR_RIGHT_OPEN :
					index == DOOR_RIGHT_OPEN ? DOOR_RIGHT_OPEN : NONE;

				if (_trigger == DOOR_RIGHT_OPEN)
					autoSound("openDoarShort");

				for (int k = 0; k < _vFrameTile.size(); k++)
				{
					if (_vFrameTile[k].isMaxframe || _vFrameTile[k].isTrigger)
						continue;

					if (_isBlood && (TRIGGER)index == PALM && _vFrameTile[k].keyName == "손바닥")
					{
						//트리거를 발동한다.
						_vFrameTile[k].isTrigger = true;
						_trigger = (TRIGGER)index;
						STAGEMEMORYMANAGER->setIsPalmRight(true);
						autoSound("피1");
					}
					//만약 타일의 프레임 이미지가 문.. 머시기라면
					if (_tiles[index].keyName.find("문") != string::npos && _tiles[index].keyName.size() <= 3)
					{
						//해당 프레임 이미지를 찾아서
						for (int k = 0; k < _vFrameTile.size(); k++)
						{
							if (_vFrameTile[k].indexX == i && _vFrameTile[k].indexY == j && _vFrameTile[k].keyName == _tiles[index].keyName)
							{
								if (STAGEMEMORYMANAGER->getIsBearComing() && STAGEMEMORYMANAGER->getIsBearComing2())
									_trigger = DOOR_UP_OPEN2;
								else if (!STAGEMEMORYMANAGER->getIsBearComing())
									_trigger = DOOR_UP_OPEN;
								//트리거를 발동한다.
								_vFrameTile[k].isTrigger = true;
								autoSound("openDoarLong");
							}
						}
					}
				}
				break;
			}

			//곰돌이 팔다리를 자른 상태이며 에너미 곰1이 나오지 않았다면
			//해당 타일의 속성이 BEARCOM이라면
			if (STAGEMEMORYMANAGER->getIsScissors() && !STAGEMEMORYMANAGER->getIsBearComing() && !STAGEMEMORYMANAGER->getIsBearComing2()&&(TRIGGER)index == BEARCOM || (TRIGGER)index == 560)
			{
				//에너미를 생성한다.
				_bear = new bear;
				_bear->init(319% TILEX, 319 / TILEX);
				_playerTile = new astarTile;
				_enemyTile = new astarTile;
				_dead = new DeadManager;
				_dead->init();
				_dead->setPlayerAddress(_player);
				for (int y = 0; y < TILEX*TILEY; y++)
					_objTile[y] = new astarTile;
				bossLocX = 319 % TILEX;
				bossLocY = 319 / TILEX;

				firstFloorStage::objectLocation();

				_trigger = BEARCOM;
				break;
			}
			
		}
	}
}

void scissorsRoom::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/1f_scissorsRoom.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute == PLAYER)
		{
			_player->setStart(i%TILEX, i / TILEX);
		}
	}
	CloseHandle(file);
}

void scissorsRoom::getMemory()
{
	for (int k = 0; k < _vFrameTile.size(); k++)
	{
		//손바닥이 트리거가 발동되지 않은 경우 이 씬에서는 손바닥을 보여주지 않는다.
		if (STAGEMEMORYMANAGER->getIsPalmLeft() || STAGEMEMORYMANAGER->getIsPalmRight()) _isBlood = true;

		//해당 타일에 가위가 존재하며 가위를 사용하지 않은 상태라면
		if (_vFrameTile[k].keyName == "가위" && STAGEMEMORYMANAGER->getIsScissors())
		{
			//트리거가 이미 발동되었던 상태로 셋팅한다.
			_vFrameTile[k].isMaxframe = true;
		}
		if (_vFrameTile[k].keyName == "손바닥" && STAGEMEMORYMANAGER->getIsPalm())
		{
			//트리거가 이미 발동되었던 상태로 셋팅한다.
			_vFrameTile[k].isMaxframe = true;
		}
	}
}
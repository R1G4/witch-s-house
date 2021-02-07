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

	int temp = (int)"sdas";
	//타일 불러오기
	load();

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	firstFloorStage::init();

	_trigger = NONE;

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
		break;
	case scissorsRoom::SCISSORS:
 		STAGEMEMORYMANAGER->setIsScissors(true);
		//해당 프레임 이미지를 찾아서
		for (int k = 0; k < _vFrameTile.size(); k++)
		{
			if (_vFrameTile[k].keyName.find("문") != string::npos && _vFrameTile[k].keyName.size() <= 3)
			{
				//트리거를 발동한다.
				_vFrameTile[k].isTrigger = true;
				_vScript = TEXTMANAGER->loadFile("dialog/1f/1f_scissorsRoom.txt");
			}
		}
		_trigger = DELAY;
		break;	
	case scissorsRoom::BEARCOM:
		STAGEMEMORYMANAGER->setIsBearComing(true); 
		//STAGEMEMORYMANAGER->setIsBearComing2(true);
		_trigger = NONE;
		break;
	case scissorsRoom::DELAY:
		firstFloorStage::setAlpha();
		_player->setState(CHR_IDLE);
		_delay++;
		if (_delay % 60 == 0)
		{
			TEXTMANAGER->clearScript();
			if (_vScript.size() > 0) _vScript.clear();
			_trigger = NONE;
		}
		break;
	case scissorsRoom::DOOR_UP_OPEN:
		firstFloorStage::setAlpha();
		_isBlood = true;
		break;
	case scissorsRoom::DOOR_UP_OPEN2:
		SCENEMANAGER->changeScene("stairs_2F");
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
							if (_vFrameTile[k].isMaxframe || _vFrameTile[k].isTrigger)
								continue;

							if (_vFrameTile[k].keyName == "가위")
							{
								//트리거를 발동한다.
								_vFrameTile[k].isTrigger = true;
								_trigger = SCISSORS;
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
				cout << "x: " << i << "  y: " << j << "  index: " << index << endl;

				_trigger = index == 564 ? DOOR_RIGHT_OPEN :
					index == DOOR_RIGHT_OPEN ? DOOR_RIGHT_OPEN : NONE;

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
								else
									_trigger = DOOR_UP_OPEN;
								//트리거를 발동한다.
								_vFrameTile[k].isTrigger = true;
							}
						}
					}
				}
				break;
			}

			//곰돌이 팔다리를 자른 상태라면
			if (STAGEMEMORYMANAGER->getIsScissors() &&
				!STAGEMEMORYMANAGER->getIsBearComing()&&
				!STAGEMEMORYMANAGER->getIsBearComing2()&&
				(TRIGGER)index == BEARCOM || (TRIGGER)index == 560)
			{
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
			cout << i % TILEX << "&&, " << i / TILEX << endl;
			_player->setStart(i%TILEX, i / TILEX);
		}
	}
	CloseHandle(file);
}

void scissorsRoom::getMemory()
{
	for (int k = 0; k < _vFrameTile.size(); k++)
	{
		if (STAGEMEMORYMANAGER->getIsPalmLeft() || STAGEMEMORYMANAGER->getIsPalmRight()) _isBlood = true;

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
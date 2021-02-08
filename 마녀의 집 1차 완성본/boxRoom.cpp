#include "stdafx.h"
#include "boxRoom.h"
#include "Player.h"


boxRoom::boxRoom()
{
}


boxRoom::~boxRoom()
{
}

HRESULT boxRoom::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
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

	//데드 변수를 초기화한다.
	_dead = new DeadManager;
	_dead->init();
	_dead->setPlayerAddress(_player);
	return S_OK;
}

void boxRoom::release()
{
	firstFloorStage::release();
}

void boxRoom::update()
{
	//프레임 인덱스 셋팅
	setFrameIndex();

	//트리거 상태에 따른 호출 및 설정
	switch (_trigger)
	{
	case boxRoom::NONE:
		firstFloorStage::update();
		firstFloorStage::setAlpha();
		Collision();
		break;
	case boxRoom::DOOR_LEFT_OPEN:
		firstFloorStage::sceneChange("hallway", CHRDIREC_LEFT, LOCATION_1);
		break;
	case boxRoom::BOX:
		firstFloorStage::setAlpha();
		STAGEMEMORYMANAGER->setIsBox(true);
		_trigger = DELAY;
		break;
	case boxRoom::DELAY:
		firstFloorStage::setAlpha();
		_player->setState(CHR_IDLE);
		_delay++;
		if (_delay % 70 == 0)
			_trigger = NONE;
		break;
	case boxRoom::BEAR_PICKUP:
		STAGEMEMORYMANAGER->setIsBearPickUp(true);
		_trigger = NONE;
		break;
	case boxRoom::READ:
		if (KEYMANAGER->isOnceKeyUp(VK_SPACE))
		{
			switch (_readCnt)
			{
			case boxRoom::FIRST:
				autoSound("openBook");
				_vScript = TEXTMANAGER->loadFile("dialog/1f/1f_boxRoom.txt");
				_readCnt = SECOND;
				break;
			case boxRoom::SECOND:
				autoSound("nextPage");
				TEXTMANAGER->setNextScript(true);
				_readCnt = THIRD;
				break;
			case boxRoom::THIRD:
				autoSound("nextPage");
				TEXTMANAGER->setNextScript(true);
				_readCnt = FOURTH;
				break;
			case boxRoom::FOURTH:
				autoSound("nextPage");
				TEXTMANAGER->setNextScript(true);
				_readCnt = END;
				break;
			case boxRoom::END:
				TEXTMANAGER->clearScript();
				_trigger = NONE;
				_vScript.clear();
				break;
			}
		}
		break;
	case boxRoom::THORN:
		firstFloorStage::update();
		firstFloorStage::setAlpha();
		_dead->setDead(DEAD_THORN);
		_dead->update();

		_delay++;
		//풍부함을 위해서.. 여러번 재생한다
		if (!(_delay % 57))
			autoSound("여자");
		break;
	default:
		_trigger = NONE;
		firstFloorStage::update();
		firstFloorStage::setAlpha();
		Collision();
		break;
	}

	firstFloorStage::cameraUpdate();
}

void boxRoom::render()
{
	//배경 랜더
	IMAGEMANAGER->FindImage("배경13")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경13"),
		Vector2(IMAGEMANAGER->FindImage("배경13")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("배경13")->GetSize().y / 2));

	firstFloorStage::render();
}

void boxRoom::Collision()
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
				if (_readCnt != END && (TRIGGER)index == READ && SelectionForm(L"읽는다.", L"읽지 않는다") && _fromSelected == LEFT)
				{
					_trigger = READ;
				}
				//텍스를 넣는 동시에 폼 실행
				if ((TRIGGER)index == THORN && SelectionForm(L"장농을 열본다.", L"그만둔다.") && _fromSelected == LEFT)
				{
					//사운드 추가 여자 비웃기
					_trigger = THORN;
					autoSound("여자");
				}
			}

			//어느 타일과 플레이어 상호작용 렉트가 충돌하였다면
			if (IntersectRectToRect(&_tiles[index].rc, &_player->getSearchRc()))
			{
				//타일의 인덱스에 해당 아이템이 존재 하고 획득을 시도한다면
				if (!STAGEMEMORYMANAGER->getIsBearPut() && !STAGEMEMORYMANAGER->getIsBearPickUp())

					if ((TRIGGER)index == BEAR_PICKUP)
					{
						//텍스를 넣는 동시에 폼 실행
						if (SelectionForm(L"곰인형을 줍는다.", L"아무것도 하지 않는다.") && _fromSelected == LEFT)
						{
							//아이템 매니저 - 아이템 획득
							//아이템 키(image.find(key))값 혹은 아이템 파일명을 넣는다
							//보유 아이템이 이미 존재하거나 매니저 안에 키값이 존재 하지 않는 등 실패할 경우 false를 반환한다. true일 경우 추가(획득)
							if (!ITEMMANAGER->addItem(_tiles[index].keyName)) continue;
							_tiles[index].obj = OBJ_NONE;
							_tiles[index].keyName = "";
							_trigger = BEAR_PICKUP;
							autoSound("getItem");
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
				//그외 트리거 받아오기
				_trigger = index == 556 ? DOOR_LEFT_OPEN :
					index == DOOR_LEFT_OPEN ? DOOR_LEFT_OPEN : NONE;

				if (_trigger == DOOR_LEFT_OPEN || _trigger == DOOR_LEFT_OPEN)
					autoSound("openDoarLong");
				for (int k = 0; k < _vFrameTile.size(); k++)
				{
					if (_vFrameTile[k].isMaxframe || _vFrameTile[k].isTrigger)
						continue;

					if (!STAGEMEMORYMANAGER->getIsBox() && ((TRIGGER)index == BOX || index == 500) && _vFrameTile[k].keyName == "상자쓰러짐")
					{
						//트리거를 발동한다.
						_vFrameTile[k].isTrigger = true;
						_trigger = BOX;
						autoSound("박스");
					}
				}
				break;
			}
		}
	}
}

void boxRoom::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/1f_boxRoom.map", GENERIC_READ, NULL, NULL,
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

void boxRoom::getMemory()
{
	if (STAGEMEMORYMANAGER->getIsBearPickUp())
	{
		_tiles[BEAR_PICKUP].obj = OBJ_NONE;
		_tiles[BEAR_PICKUP].keyName = "";

		for (int k = 0; k < _vFrameTile.size(); k++)
		{
			if (_vFrameTile[k].keyName == "상자쓰러짐")
			{
				//트리거가 이미 발동되었던 상태로 셋팅한다.
				_vFrameTile[k].isMaxframe = true;
			}
		}
	}
}
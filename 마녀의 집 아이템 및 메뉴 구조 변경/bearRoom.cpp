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

	//프레임 이미지 프레임 셋팅
	setFrameIndex();
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

	//트리거 상태에 따른 호출 및 설정
	switch (_trigger)
	{
	case bearRoom::NONE:
		firstFloorStage::update();
		firstFloorStage::setAlpha();
		Collision();
		break;
	case bearRoom::DOOR_LEFT_OPEN:
		firstFloorStage::sceneChange("hallway", CHRDIREC_LEFT, LOCATION_2);
		break;
	case bearRoom::BEAR_PUT:
		STAGEMEMORYMANAGER->setIsBearPut(true);
		_trigger = NONE;
		break;	
	case bearRoom::BEAR_ACCESS:
		_bearAccess = true;
		_player->setState(CHR_IDLE);
		int index;
		for (int i = 0; i < _vFrameTile.size(); i++)
		{
			if (_vFrameTile[i].keyName == "바구니곰")
			{
				index = i;
				break;
			}
		}
		_vFrameTile[index].indexX = (cnt % 3) + 1;
		_vFrameTile[index].rc.left -= 2;
		_vFrameTile[index].rc.right -= 2;
		cnt ++;

		if (cnt > 10)
		{
			_trigger = NONE;
			cnt = 0;
		}
		break;
	case bearRoom::READ:
		firstFloorStage::setAlpha();
		_vScript = TEXTMANAGER->loadFile("dialog/1f/1f_bearRoom.txt");

		if (KEYMANAGER->isOnceKeyUp(VK_SPACE) || KEYMANAGER->isOnceKeyUp('x'))
		{
			TEXTMANAGER->clearScript();
			_trigger = NONE;
			_vScript.clear();
		}
		break;
	default:
		_trigger = NONE;
		firstFloorStage::setAlpha();
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

			//어느 타일과 플레이어 상호작용 렉트가 충돌하였다면
			if (IntersectRectToRect(&_tiles[index].rc, &_player->getSearchRc()))
			{
				if ((TRIGGER)index == READ && KEYMANAGER->isOnceKeyUp(VK_SPACE))
					_trigger = READ;

				//타일의 인덱스에 해당 아이템이 존재 하고 획득을 시도한다면
				if(STAGEMEMORYMANAGER->getIsScissors() && !STAGEMEMORYMANAGER->getIsBearPut() && STAGEMEMORYMANAGER->getIsBearPickUp())
				if ((TRIGGER)index == BEAR_PUT)
				{
					for (int k = 0; k < _vFrameTile.size(); k++)
					{
						if (_vFrameTile[k].keyName == "바구니곰")
						{
							if (_vFrameTile[k].isMaxframe || _vFrameTile[k].isTrigger)	continue;

							//텍스를 넣는 동시에 폼 실행
							if (SelectionForm(L"곰인형을 넣는다", L"아무것도 하지 않는다.") && _fromSelected == LEFT)
							{
								//아이템 매니저 - 아이템 사용
								//아이템 키(image.find(key))값 혹은 아이템 파일명을 넣는다
								//플레이어가 아이템 사용을 시도한다면 보유 아이템은 제거된다.
								//아이템 사용을 시도하지 않거나 해당 아이템을 갖고 있지 않다면 false를 반환한다. true일 경우 제거(사용)
								if (!ITEMMANAGER->useItem(11)) continue;

								//사용을 하였으면 트리거를 발동한다.
								_vFrameTile[k].isTrigger = true;
								_trigger = BEAR_PUT;
							}
							break;
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

				if ((index == BEAR_ACCESS || index == 557) && !_bearAccess)
				{
					if (STAGEMEMORYMANAGER->getIsScissors() && STAGEMEMORYMANAGER->getIsBearPut() && STAGEMEMORYMANAGER->getIsBearPickUp())
						_trigger = BEAR_ACCESS;
				}

				//그외 트리거 받아오기
				if(index != BEAR_PUT && _trigger != READ && _trigger != BEAR_ACCESS)
					_trigger = index == 556 ? DOOR_LEFT_OPEN :
							   index == DOOR_LEFT_OPEN ? DOOR_LEFT_OPEN : NONE;

				break;
			}
		}
	}
}

void bearRoom::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/1f_bearRoom.map", GENERIC_READ, NULL, NULL,
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

void bearRoom::getMemory()
{
	for (int k = 0; k < _vFrameTile.size(); k++)
	{
		if (_vFrameTile[k].keyName == "바구니곰" && STAGEMEMORYMANAGER->getIsBearPut())
		{
			//트리거가 이미 발동되었던 상태로 셋팅한다.
			_vFrameTile[k].isMaxframe = true;
		}
		if (_vFrameTile[k].keyName == "손바닥" && (STAGEMEMORYMANAGER->getIsPalmLeft() || (STAGEMEMORYMANAGER->getIsPalmRight())))
		{
			//트리거가 이미 발동되었던 상태로 셋팅한다.
			_vFrameTile[k].isMaxframe = true;
		}
	}
}
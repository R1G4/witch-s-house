#include "stdafx.h"
#include "diningRoom_5f.h"
#include "Player.h"

HRESULT diningRoom_5f::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	SOUNDMANAGER->play("다이닝룸");
	_real_location1 = DININGROOM;
	_player->setDirec(_chrdirection);

	//타일 불러오기
	load(_location);

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	fifthFloorStage::init();

	getMemory();
	_isClock = false;
	_sound = false;
	return S_OK;
}

void diningRoom_5f::release()
{
	SOUNDMANAGER->stop("다이닝룸");
	_sound = false;
}

void diningRoom_5f::update()
{
	if (!_isClick && !_isStopToRead)
	{
		fifthFloorStage::update();

		setFrameIndex();

		//카메라 관련 업데이트
		cameraUpdate();
		setTrigger();
	}
	setChoiceScene(); 
	
	if (_isStopToRead)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// 클릭행동 트리거
			_isStopToRead = TEXTMANAGER->setNextScript(true);
		if (_sound_item && !_isStopToRead)
		{
			SOUNDMANAGER->play("getItem");
			_sound_item = false;
		}
		if (!_isStopToRead && _isClock)
		{
			_co = SECOND;
			_isClick = true;
		}
	}
}

void diningRoom_5f::render()
{	
	//배경 랜더
	IMAGEMANAGER->FindImage("배경62")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경62"),
		Vector2(IMAGEMANAGER->FindImage("배경62")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("배경62")->GetSize().y / 2));

	fifthFloorStage::render();

	if (_isStopToRead)
		TEXTMANAGER->renderText();

	if (_isClick)
	{
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 250, camera.y));	//왼쪽
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x + 250, camera.y));	//오른쪽

		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 300, 345, L"만진다.", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 117, 345, L"아무것도 하지 않는다.", RGB(255, 255, 255), 0.85f, 27);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
	}
}

void diningRoom_5f::Collision()
{
}

void diningRoom_5f::load(LOCATION _location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/5f_diningRoom.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute != PLAYER) continue;

		//초기 위치를 잡아준다.
		switch (_location)
		{
		case LOCATION_DEFAULT: default:
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}
	}
	CloseHandle(file);
}

void diningRoom_5f::setTrigger()
{
	FloatRect pFrc = _player->getPlayerFrc();
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// 클릭행동 트리거
	{
		if (IntersectRectToRect(&_tiles[Y_FLOWER_1].rc, &pFrc) &&
			STAGEMEMORYMANAGER->getIsFlowerDead())
		{
			SOUNDMANAGER->play("여자비웃음");
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_flower_1_1.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		else if (IntersectRectToRect(&_tiles[Y_FLOWER_1].rc, &pFrc))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_flower_1.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		if (IntersectRectToRect(&_tiles[Y_FLOWER_2].rc, &pFrc) &&
			STAGEMEMORYMANAGER->getIsFlowerDead())
		{
			SOUNDMANAGER->play("여자비웃음");
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_flower_2_1.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		else if (IntersectRectToRect(&_tiles[Y_FLOWER_2].rc, &pFrc))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_flower_2.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		if (IntersectRectToRect(&_tiles[Y_FLOWER_3].rc, &pFrc) &&
			STAGEMEMORYMANAGER->getIsFlowerDead() &&
			STAGEMEMORYMANAGER->getIsGetSkul3())
		{
			SOUNDMANAGER->play("여자비웃음");
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_flower_2_1.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		else if (IntersectRectToRect(&_tiles[Y_FLOWER_3].rc, &pFrc) &&
			STAGEMEMORYMANAGER->getIsFlowerDead())
		{
			SOUNDMANAGER->play("여자비웃음");
			STAGEMEMORYMANAGER->setIsGetSkul3(true);
			_sound_item = true;
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_flower_3_1.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		else if (IntersectRectToRect(&_tiles[Y_FLOWER_3].rc, &pFrc))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_flower_3.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		if (IntersectRectToRect(&_tiles[Y_FLOWER_4].rc, &pFrc) &&
			STAGEMEMORYMANAGER->getIsFlowerDead())
		{
			SOUNDMANAGER->play("여자비웃음");
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_flower_4_1.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		else if (IntersectRectToRect(&_tiles[Y_FLOWER_4].rc, &pFrc))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_flower_4.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		FloatRect pSRc = _player->getSearchRc();
		if (IntersectRectToRect(&_tiles[CLOCK].rc, &pSRc) && _player->getPdirec() == CHRDIREC_UP)
		{
			_co = FIRST;
			switch (_co)
			{
			case FIRST:
				_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_clock.txt");
				_isStopToRead = true;
				_isClock = true;
				break;
			case SECOND: default:
				break;
			}
		}
		if (IntersectRectToRect(&_tiles[LIGHT].rc, &pSRc))
		{
			_sound_item = true;
			STAGEMEMORYMANAGER->setIsGetSkul1(true);
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_light.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		if (IntersectRectToRect(&_tiles[DOORTODININGROOMITEM].rc, &pFrc))
		{
			_sound_item = true;
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_door.txt");
			STAGEMEMORYMANAGER->setIsKey(true);
			_isStopToRead = true;
			_isClock = false;
		}
	}

	if (IntersectRectToRect(&_tiles[DOORTOGARDEN].rc, &pFrc) ||
		IntersectRectToRect(&_tiles[DOORTOGARDEN + TILEX].rc, &pFrc))
	{
		if (!_sound)
			SOUNDMANAGER->play("openDoarLong");
		if (STAGEMEMORYMANAGER->getIsGetSkul3() && !_sound)
			SOUNDMANAGER->play("여자");
		_sound = true;
		_isChangeScene = true;
		sceneChange("garden_5f", CHRDIREC_RIGHT, LOCATION_1);
	}
	if (IntersectRectToRect(&_tiles[DROPFLOWER].rc, &pFrc))
	{
		STAGEMEMORYMANAGER->setIsFlowerpot(true);
		_vFrameTile[2].isTrigger = true;
	}

}

void diningRoom_5f::setChoiceScene()
{
	if (_isClick)	// 시계 관련 이벤트
	{
		_correct_rc = RectMakePivot(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
		rcAlphaChange();
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			SOUNDMANAGER->play("cursor");
			_rc = RectMakePivot(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			SOUNDMANAGER->play("cursor");
			_rc = RectMakePivot(Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			_isClick = false;
			if (IntersectRectToRect(&_rc, &_correct_rc))
			{
				SOUNDMANAGER->play("쾅");
				_dead->setDead(DEAD_CLOCK);
				_dead->setLocXY(_tiles[CLOCK].rc.left, _tiles[CLOCK].rc.top);
				_tiles[CLOCK].obj = OBJ_NONE;
				_player->~Player();
				_isDead = true;
			}
			else
				SOUNDMANAGER->play("cursor");
		}
	}
}

void diningRoom_5f::getMemory()
{
	for (int k = 0; k < _vFrameTile.size(); k++)
	{
		if (!STAGEMEMORYMANAGER->getIsFlowerpot()) continue;

		if (_vFrameTile[k].keyName == "꽃병프레임")
		{
			//트리거가 이미 발동되었던 상태로 셋팅한다.
			_vFrameTile[k].isMaxframe = true;
		}
	}
}

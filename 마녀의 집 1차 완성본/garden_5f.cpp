#include "stdafx.h"
#include "garden_5f.h"
#include "player.h"

HRESULT garden_5f::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	SOUNDMANAGER->play("정원");
	_real_location1 = GARDEN;
	_player->setDirec(_chrdirection);

	//타일 불러오기
	load(_location);

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	fifthFloorStage::init();

	_x = WINSIZEX / 2 - 250;
	_y = WINSIZEY / 2;
	_rc = RectMakePivot(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
	_correct_rc = RectMakePivot(Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
	_co = FIRST;
	_sound = false;
	return S_OK;
}

HRESULT garden_5f::init(int x, int y, CHRDIRECTION _direction)
{
	SOUNDMANAGER->play("정원");
	_real_location1 = GARDEN;
	_player->setDirec(_direction);

	//타일 불러오기
	load();
	fifthFloorStage::init();


	_player->setPLocaX(x);
	_player->setPLocaY(y);
	_player->setDirec(_direction);

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());

	_x = WINSIZEX / 2 - 250;
	_y = WINSIZEY / 2;
	_rc = RectMakePivot(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
	_correct_rc = RectMakePivot(Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
	_co = FIRST;
	_sound = false;
	return S_OK;
}

void garden_5f::release()
{
	SOUNDMANAGER->stop("정원");
	_sound = false;
}

void garden_5f::update()
{
	if (!_isClick && !_isStopToRead)
	{
		fifthFloorStage::update();

		setFrameIndex();

		//카메라 관련 업데이트
		cameraUpdate();

		if(!_isDead)
			setTrigger();

		if (STAGEMEMORYMANAGER->getIsFlowerDead())
		{
			_count++;
			if (_count % 4 == 0)
			{
				_frame++;
				if (_frame > 8)
				{
					_frame = 8;
					return;
				}
				_deadFlowerText = true;
			}
		}

		_count_cat++;
		if (_count_cat % 10 == 0)
		{
			_frame_cat++;
			if (_frame_cat > 7)
			{
				_frame_cat = 0;
				_count_cat = 0;
			}
		}

		if (_frame == 8 && _deadFlowerText)
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_garden_flower_1.txt");
			_isStopToRead = true; 
			_deadFlowerText = false;
		}
	}
	setChoiceScene();
	
	if (_isStopToRead)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// 클릭행동 트리거
			_isStopToRead = TEXTMANAGER->setNextScript(true);
	}
}

void garden_5f::render()
{	
	//배경 랜더
	IMAGEMANAGER->FindImage("배경60")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경60"),
		Vector2(IMAGEMANAGER->FindImage("배경60")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("배경60")->GetSize().y / 2));

	if (STAGEMEMORYMANAGER->getIsFlowerDead())
		CAMERAMANAGER->FrameRender(IMAGEMANAGER->FindImage("하얀꽃죽음"), Vector2(_tiles[FLOWER].rc.left, _tiles[FLOWER].rc.top), _frame, 0);
	CAMERAMANAGER->FrameRender(IMAGEMANAGER->FindImage("고양이1"), Vector2(_tiles[NEKO].rc.left + 24, _tiles[NEKO].rc.top + 6), _frame_cat, 0);
	
	fifthFloorStage::render();

	
	if (_isStopToRead)
		TEXTMANAGER->renderText();

	if (_isClick)
	{
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 250, camera.y));	//왼쪽
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x + 250, camera.y));	//오른쪽
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x, camera.y - 150));	//위
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x, camera.y + 150));	//아래

		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 300, 345, L"뜯는다.", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 117, 345, L"아무것도 하지 않는다.", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 50, 195, L"뽑는다.", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 50, 495, L"꺾는다.", RGB(255, 255, 255), 0.85f, 27);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
	}
	
}

void garden_5f::Collision()
{
}

void garden_5f::load(LOCATION _location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/5f_garden.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute != PLAYER) continue;

		//초기 위치를 잡아준다.
		switch (_location)
		{
		case LOCATION_1:
			_player->setStart((DOORTODININGROOM + 1) % TILEX, (DOORTODININGROOM + 1) / TILEX);
			break;
		case LOCATION_2:
			_player->setStart((DOORTOGARDENTOBOSS + TILEX) % TILEX, (DOORTOGARDENTOBOSS + TILEX) / TILEX);
			break;		
		case LOCATION_3:
			_player->setStart((DOORTOPRISON - 1) % TILEX, (DOORTOPRISON - 1) / TILEX);
			break;
		case LOCATION_DEFAULT: default:
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}
	}
	CloseHandle(file);
}

void garden_5f::setTrigger()
{
	if (IntersectRectToRect(&_tiles[NEKO].rc, &_player->getSearchRc()))
	{
		setSave();
	}

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// 클릭행동 트리거
	{
		if (IntersectRectToRect(&_tiles[FLOWER].rc, &_player->getSearchRc()) && 
			STAGEMEMORYMANAGER->getIsPotion() &&
			STAGEMEMORYMANAGER->getIsRedFlower1() &&
			STAGEMEMORYMANAGER->getIsRedFlower2() &&
			STAGEMEMORYMANAGER->getIsRedFlower3()
			)
		{
			SOUNDMANAGER->play("비명소리");
			_flowerDead = true;
			STAGEMEMORYMANAGER->setIsFlowerDead(true);
		}
		else if (IntersectRectToRect(&_tiles[FLOWER].rc, &_player->getSearchRc()))
		{
			switch (_co)
			{
			case FIRST:
				_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_garden_flower.txt");
				_isStopToRead = true;
				_co = SECOND;
				break;
			case SECOND:
				_isClick = true;
				break;
			}
		}
		if (IntersectRectToRect(&_tiles[TREE].rc, &_player->getSearchRc()) ||
			IntersectRectToRect(&_tiles[TREE + 1].rc, &_player->getSearchRc()))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_garden_tree.txt");
			_isStopToRead = true;
		}
		if (IntersectRectToRect(&_tiles[TEE].rc, &_player->getSearchRc()))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_garden_tee.txt");
			_isStopToRead = true;
		}
		if (IntersectRectToRect(&_tiles[DOORTOGARDENTOBOSS].rc, &_player->getSearchRc()))
		{
			SOUNDMANAGER->play("openDoarShort");
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_garden_door.txt");
			_isStopToRead = true;
		}
	}



	if (IntersectRectToRect(&_tiles[DOORTOGARDENTOBOSS].rc, &_player->getPlayerFrc()) &&
		STAGEMEMORYMANAGER->getIsLever())
	{
		if (!_sound)
			SOUNDMANAGER->play("openDoarLong");
		_sound = true;
		_isChangeScene = true;
		_vFrameTile[1].isTrigger = true;
		sceneChange("gardenToBoss_5f", CHRDIREC_UP, LOCATION_DEFAULT);
	}
	if (IntersectRectToRect(&_tiles[DOORTOPRISON].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISON + TILEX].rc, &_player->getPlayerFrc()))
	{
		if (!_sound)
			SOUNDMANAGER->play("철문");
		_sound = true;
		_isChangeScene = true;
		sceneChange("prison_5f", CHRDIREC_RIGHT, LOCATION_DEFAULT);
	}
	if (IntersectRectToRect(&_tiles[DOORTODININGROOM].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTODININGROOM + TILEX].rc, &_player->getPlayerFrc()))
	{
		if (!_sound)
			SOUNDMANAGER->play("openDoarLong");
		_sound = true;
		_isChangeScene = true;
		sceneChange("diningRoom_5f", CHRDIREC_LEFT, LOCATION_DEFAULT);
	}
}

void garden_5f::setChoiceScene()
{
	if (_isClick)
	{
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
		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			SOUNDMANAGER->play("cursor");
			_rc = RectMakePivot(Vector2(WINSIZEX / 2, WINSIZEY / 2 - 150), Vector2(270, 75), Pivot::Center);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			SOUNDMANAGER->play("cursor");
			_rc = RectMakePivot(Vector2(WINSIZEX / 2, WINSIZEY / 2 + 150), Vector2(270, 75), Pivot::Center);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			_isClick = false;
			if (IntersectRectToRect(&_rc, &_correct_rc))
				SOUNDMANAGER->play("cursor");
			else 
			{
				SOUNDMANAGER->play("blood");
				_dead->setDead(DEAD_FLOWER);
				_isDead = true;
			}
		}
	}
}

#include "stdafx.h"
#include "prison_5f_skul.h"
#include "Player.h"

HRESULT prison_5f_skul::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	SOUNDMANAGER->play("감옥1");
	_real_location1 = PRISON_SKUL;
	_player->setDirec(_chrdirection);

	//타일 불러오기
	load(_location);

	_tiles[SKUL_1].obj = OBJ_NONE;
	_tiles[SKUL_2].obj = OBJ_NONE;
	_tiles[SKUL_3].obj = OBJ_NONE;
	_tiles[SKUL_4].obj = OBJ_NONE;

	getMemory();

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	fifthFloorStage::init();
	_sound = false;
	return S_OK;
}

void prison_5f_skul::release()
{
	SOUNDMANAGER->stop("감옥1");
	_sound = false;
}

void prison_5f_skul::update()
{
	if(!_isStopToRead)
	{
		fifthFloorStage::update();
		setFrameIndex();

		//카메라 관련 업데이트
		cameraUpdate();
		setTrigger();
	}
	else
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// 클릭행동 트리거
		{
			_isStopToRead = TEXTMANAGER->setNextScript(true);

			if (_isSkul)
			{
				SOUNDMANAGER->play("getItem");
				_tiles[SKUL_5].obj = OBJ_NONE;
			}
		}
	}
}

void prison_5f_skul::render()
{	
	//배경 랜더
	IMAGEMANAGER->FindImage("배경67")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경67"),
		Vector2(IMAGEMANAGER->FindImage("배경67")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("배경67")->GetSize().y / 2));

	fifthFloorStage::render();

	if (_isStopToRead)
		TEXTMANAGER->renderText();
}

void prison_5f_skul::Collision()
{
}

void prison_5f_skul::load(LOCATION _location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/5f_prison_1.map", GENERIC_READ, NULL, NULL,
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
			_player->setStart(i % TILEX, i / TILEX);
			break;
		}
	}
	CloseHandle(file);
}

void prison_5f_skul::setTrigger()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// 클릭행동 트리거
	{
		if (IntersectRectToRect(&_tiles[SKUL_1].rc, &_player->getSearchRc()) &&
			STAGEMEMORYMANAGER->getIsGetSkul1())
		{
			SOUNDMANAGER->play("해골두기");
			STAGEMEMORYMANAGER->setIsSkul1(true);
			_tiles[SKUL_1].obj = OBJ_LOOK;
		}
		if (IntersectRectToRect(&_tiles[SKUL_2].rc, &_player->getSearchRc()) &&
			STAGEMEMORYMANAGER->getIsGetSkul2())
		{
			SOUNDMANAGER->play("해골두기");
			STAGEMEMORYMANAGER->setIsSkul2(true);
			_tiles[SKUL_2].obj = OBJ_LOOK;
		}
		if (IntersectRectToRect(&_tiles[SKUL_3].rc, &_player->getSearchRc()) &&
			STAGEMEMORYMANAGER->getIsGetSkul3())
		{
			SOUNDMANAGER->play("해골두기");
			STAGEMEMORYMANAGER->setIsSkul3(true);
			_tiles[SKUL_3].obj = OBJ_LOOK;
		}
		if (IntersectRectToRect(&_tiles[SKUL_4].rc, &_player->getSearchRc()) && 
			STAGEMEMORYMANAGER->getIsGetSkul4())
		{
			SOUNDMANAGER->play("해골두기");
			STAGEMEMORYMANAGER->setIsSkul4(true);
			_tiles[SKUL_4].obj = OBJ_LOOK;
		}
		if (IntersectRectToRect(&_tiles[SKUL_5].rc, &_player->getSearchRc()))
		{
			STAGEMEMORYMANAGER->setIsSkul5(true);
			STAGEMEMORYMANAGER->setIsGetSkul4(true);
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_skul.txt");
			_isStopToRead = true;
			_isSkul = true;
		}
		if (IntersectRectToRect(&_tiles[LEVER].rc, &_player->getSearchRc()) && 
			_tiles[SKUL_1].obj == OBJ_LOOK &&
			_tiles[SKUL_2].obj == OBJ_LOOK &&
			_tiles[SKUL_3].obj == OBJ_LOOK &&
			_tiles[SKUL_4].obj == OBJ_LOOK)
		{
			SOUNDMANAGER->play("스위치");
			STAGEMEMORYMANAGER->setIsLever(true);
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_lever_1.txt");
			_isStopToRead = true;
		}
		else if (IntersectRectToRect(&_tiles[LEVER].rc, &_player->getSearchRc()))
		{
			SOUNDMANAGER->play("스위치");
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_lever.txt");
			_isStopToRead = true;
		}
	}

	if (IntersectRectToRect(&_tiles[DOORTOPRISON].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISON + 1].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISON - 1].rc, &_player->getPlayerFrc()))
	{
		if (!_sound)
			SOUNDMANAGER->play("철문");
		_sound = true;
		_isChangeScene = true;
		sceneChange("prison_5f", CHRDIREC_LEFT, LOCATION_1);
	}
}

void prison_5f_skul::getMemory()
{
	if (STAGEMEMORYMANAGER->getIsSkul1()) _tiles[SKUL_1].obj = OBJ_LOOK;
	if (STAGEMEMORYMANAGER->getIsSkul2()) _tiles[SKUL_2].obj = OBJ_LOOK;
	if (STAGEMEMORYMANAGER->getIsSkul3()) _tiles[SKUL_3].obj = OBJ_LOOK;
	if (STAGEMEMORYMANAGER->getIsSkul4()) _tiles[SKUL_4].obj = OBJ_LOOK;
	if (STAGEMEMORYMANAGER->getIsSkul5()) _tiles[SKUL_5].obj = OBJ_NONE;
}

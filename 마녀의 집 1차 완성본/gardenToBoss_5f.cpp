#include "stdafx.h"
#include "gardenToBoss_5f.h"
#include "Player.h"

HRESULT gardenToBoss_5f::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	SOUNDMANAGER->play("정원1");
	IMAGEMANAGER->AddFrameImage("SavePoint", L"Image/mapTool/saveCat.png", 16, 4);
	_real_location1 = GARDEN_TO_BOSS;
	_player->setDirec(_chrdirection);

	//타일 불러오기
	load(_location);

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	fifthFloorStage::init();

	// 다이얼로그 저장

	_sound = false;
	return S_OK;
}

void gardenToBoss_5f::release()
{
	SOUNDMANAGER->stop("정원1");
	_sound = false;
}

void gardenToBoss_5f::update()
{
	if (!_isStopToRead)
	{
		fifthFloorStage::update();
		setFrameIndex();

		//카메라 관련 업데이트
		cameraUpdate();
		setTrigger();

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
	}
	else
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// 클릭행동 트리거
		{
			_isStopToRead = TEXTMANAGER->setNextScript(true);
		}
	}
}

void gardenToBoss_5f::render()
{	
	//배경 랜더
	IMAGEMANAGER->FindImage("배경72")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경72"),
		Vector2(IMAGEMANAGER->FindImage("배경72")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("배경72")->GetSize().y / 2));
	
	CAMERAMANAGER->FrameRender(IMAGEMANAGER->FindImage("SavePoint"), Vector2(_tiles[NEKO].rc.left + 18, _tiles[NEKO].rc.top + 6), _frame_cat, 2);

	fifthFloorStage::render();

	if (_isStopToRead)
		TEXTMANAGER->renderText();
}

void gardenToBoss_5f::Collision()
{
}

void gardenToBoss_5f::load(LOCATION _location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/5f_garden_to_boss.map", GENERIC_READ, NULL, NULL,
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

void gardenToBoss_5f::setTrigger()
{
	FloatRect pSRc = _player->getSearchRc();
	FloatRect pFrc = _player->getPlayerFrc();

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// 클릭행동 트리거
	{
		_isStopToRead = TEXTMANAGER->setNextScript(true);

		if (IntersectRectToRect(&_tiles[BOOK].rc, &pSRc))
		{
			SOUNDMANAGER->play("openBook");
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_gardenToBossBook.txt");
			_isStopToRead = true;
		}
		if (IntersectRectToRect(&_tiles[NEKO].rc, &pSRc))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_gardenToBossNeko.txt");
			_isStopToRead = true;
		}
	}

	if (IntersectRectToRect(&_tiles[DOORTOBOSS].rc, &pFrc))
	{
		if (!_sound)
			SOUNDMANAGER->play("openDoarLong");
		SOUNDMANAGER->stop("정원1");
		_sound = true;
		_isChangeScene = true;
		_vFrameTile[0].isTrigger = true;
		sceneChange("BossStage1");
		fifthFloorStage::release();
	}
	if (IntersectRectToRect(&_tiles[DOORTOGARDEN].rc, &pFrc))
	{
		if (!_sound)
			SOUNDMANAGER->play("openDoarLong");
		_sound = true;
		_isChangeScene = true;
		sceneChange("garden_5f", CHRDIREC_DOWN, LOCATION_2);
	}
}

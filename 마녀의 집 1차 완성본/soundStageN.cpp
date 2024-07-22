#include "stdafx.h"
#include "soundStageN.h"

soundStageN::soundStageN()
{
}

soundStageN::~soundStageN()
{
}

HRESULT soundStageN::init()
{
	IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/타일.png", 7, 2);
	IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
	IMAGEMANAGER->AddFrameImage("SavePoint", L"Image/mapTool/saveCat.png", 16, 4);
	IMAGEMANAGER->AddImage("닫힌피아노", L"Image/piano-C.png");
	IMAGEMANAGER->AddImage("열린피아노", L"Image/piano-O.png");



	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

	_player = new Player;
	load();
	_player->setStart((WINSIZEX / 2 + 48 * 12) / TILESIZE, (WINSIZEY / 2 + 48 * 9) / TILESIZE);
	_player->init();
	_player->setDirec(CHRDIREC_UP);
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	CAMERAMANAGER->setCamera(camera);

	Px = WINSIZEX / 2 - 375 + 48 * 15;
	Py = WINSIZEY / 2 - 114 + 48 * 7;

	mapChange[0].x = WINSIZEX / 2 - 378 + 48 * 20;
	mapChange[0].y = WINSIZEY / 2 - 120 + 48 * 14;

	mapChange[0].rc = RectMakeCenter(mapChange[0].x, mapChange[0].y, 120, 24);
	return S_OK;
}

HRESULT soundStageN::init(CHRDIRECTION _CHRDIRECTION)
{
	if (_CHRDIRECTION == CHRDIREC_UP)
	{
		IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/타일.png", 7, 2);
		IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
		IMAGEMANAGER->AddFrameImage("SavePoint", L"Image/mapTool/saveCat.png", 16, 4);
		IMAGEMANAGER->AddImage("닫힌피아노", L"Image/piano-C.png");
		IMAGEMANAGER->AddImage("열린피아노", L"Image/piano-O.png");
		IMAGEMANAGER->AddImage("문", L"Image/obj/30.png");



		CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

		_player = new Player;
		load();
		_player->setStart((WINSIZEX / 2 + 48 * 12) / TILESIZE, (WINSIZEY / 2 + 48 * 9) / TILESIZE);
		_player->init();
		_player->setDirec(CHRDIREC_UP);
		camera.x = _player->getPlayerLocX();
		camera.y = _player->getPlayerLocY();
		CAMERAMANAGER->setCamera(camera);

		Px = WINSIZEX / 2 - 399 + 48 * 15;
		Py = WINSIZEY / 2 - 114 + 48 * 7;

		mapChange[0].x = WINSIZEX / 2 - 378 + 48 * 20;
		mapChange[0].y = WINSIZEY / 2 - 120 + 48 * 14;

		mapChange[0].rc = RectMakeCenter(mapChange[0].x, mapChange[0].y, 120, 24);
	
	}

	return S_OK;
}

void soundStageN::release()
{
}

void soundStageN::update()
{
	FloatRect pSRc = _player->getSearchRc();

	if (!_isClick && !_isStopToRead)
	{
		camera.x = _player->getPlayerLocX();
		camera.y = _player->getPlayerLocY();

		CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));
		_player->update();
		tileCollision();
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			_isStopToRead = TEXTMANAGER->setNextScript(true);
			if (IntersectRectToRect(&_tiles[P1].rc, &pSRc))
			{
				_isClick = true;
			}
			if (IntersectRectToRect(&_tiles[P2].rc, &pSRc))
			{
				_isClick = true;
			}
			if (IntersectRectToRect(&_tiles[P3].rc, &pSRc))
			{
				_isClick = true;
			}
		}
	}
	else if (_isStopToRead)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			_isStopToRead = TEXTMANAGER->setNextScript(true);
		}
	}
	if (_isClick)
	{
		_correct_rc = RectMakePivot(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
		if (_rcAlpha >= 0.9f)
			_rcAlphaChange = -0.03f;

		if (_rcAlpha <= 0.35f)
			_rcAlphaChange = 0.03f;

		_rcAlpha += _rcAlphaChange;
		if (STAGEMEMORYMANAGER->getIsGetAkbo() && !STAGEMEMORYMANAGER->getIsPiano())
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
			{
				_left = true;
				_right = false;
				_rc = RectMakePivot(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
			}
			if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
			{
				_left = false;
				_right = true;
				_rc = RectMakePivot(Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
			}
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				if (_left)
				{
					_isClick = false;
					_isStopToRead = true;
					STAGEMEMORYMANAGER->setIsPiano(true);
					STAGEMEMORYMANAGER->setIsGetAkbo(false);
					ITEMMANAGER->useItem("obj44");
					_vScript = TEXTMANAGER->loadFile("dialog/4f/4f_Piano_On.txt");
				}
				if (_right)
				{
					_isClick = false;
				}
			}
		}
		if (!STAGEMEMORYMANAGER->getIsGetAkbo() && !STAGEMEMORYMANAGER->getIsPiano())
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
			{
				_left = true;
				_right = false;
				_rc = RectMakePivot(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
			}
			if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
			{
				_left = false;
				_right = true;
				_rc = RectMakePivot(Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
			}
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				if (_left)
				{
					_isClick = false;
					_isStopToRead = true;
					_vScript = TEXTMANAGER->loadFile("dialog/4f/4f_Piano_No.txt");
				}
				if (_right)
				{
					_isClick = false;
				}
			}
		}
		if (!STAGEMEMORYMANAGER->getIsGetAkbo() && STAGEMEMORYMANAGER->getIsPiano())
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
			{
				_left = true;
				_right = false;
				_rc = RectMakePivot(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
			}
			if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
			{
				_left = false;
				_right = true;
				_rc = RectMakePivot(Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
			}
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				if (_left)
				{
					_isClick = false;
					_isStopToRead = true;
					STAGEMEMORYMANAGER->setIsPiano(false);
					STAGEMEMORYMANAGER->setIsGetAkbo(true);
					ITEMMANAGER->addItem("obj44");
					_vScript = TEXTMANAGER->loadFile("dialog/4f/4f_Piano_Close.txt");
				}
				if (_right)
				{
					_isClick = false;
				}
			}
		}
	}
		
}

void soundStageN::render()
{
	CAMERAMANAGER->render(_backGround, Vector2(_backGround->GetSize().x / 2 + 480, _backGround->GetSize().y / 2));
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (KEYMANAGER->isToggleKey(VK_TAB))
			{
				CAMERAMANAGER->renderRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::White, 1, 1);
				if (_tiles[i*TILEX + j].isCollider)
				{
					CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Red, 0.4);
				}
				if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aqua, 0.5);
				CAMERAMANAGER->renderRc(mapChange[0].rc, D2D1::ColorF::White, 1.0f, 24);
			}

		}
	}
	if (!STAGEMEMORYMANAGER->getIsPiano())
	{
		ZORDER->insert(Px, Py, "닫힌피아노", ZOBJECT);
	}
	else
		ZORDER->insert(Px, Py, "열린피아노", ZOBJECT);

	if (STAGEMEMORYMANAGER->getIsPiano() && STAGEMEMORYMANAGER->getIsOpen())
	{
		ZORDER->insert(WINSIZEX / 2 + 82 + 48 * 10, WINSIZEY / 2 - 36, "문", ZOBJECT);
	}

	

	ZORDER->insert(_player->getPlayerLocX(), _player->getPlayerLocY(), ZPLAYER);
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].obj != OBJ_NONE)
			{
				ZORDER->insert(_tiles[i*TILEX + j].rc.left, _tiles[i*TILEX + j].rc.top, _tiles[i*TILEX + j].keyName, ZOBJECT);
			}
		}
	}
	for (int i = 0; i < ZORDER->getZorder().size(); i++)
	{
		if (ZORDER->getZorder()[i].type == ZOBJECT)
		{
			CAMERAMANAGER->render(ZORDER->getZorder()[i].img, Vector2(ZORDER->getZorder()[i].x + TILESIZE / 2, ZORDER->getZorder()[i].y));
		}
		if (ZORDER->getZorder()[i].type == ZPLAYER)
		{
			_player->render();
		}
	}
	ZORDER->release();
	if (_isStopToRead)
	{
		TEXTMANAGER->renderText();
	}
	if (_isClick)
	{
		if (!STAGEMEMORYMANAGER->getIsPiano())
		{
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 250, camera.y));
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x + 250, camera.y));

			D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 325, 345, L"악보를 올린다", RGB(255, 255, 255), 0.85f, 27);
			D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 117, 345, L"아무것도 하지 않는다", RGB(255, 255, 255), 0.85f, 27);

			D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5f);
			D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
		}
		if (STAGEMEMORYMANAGER->getIsPiano())
		{
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 250, camera.y));
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x + 250, camera.y));

			D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 330, 345, L"악보를 가져온다", RGB(255, 255, 255), 0.85f, 27);
			D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 117, 345, L"아무것도 하지 않는다", RGB(255, 255, 255), 0.85f, 27);

			D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5f);
			D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
		}
	}
	
}

void soundStageN::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/4F_NROOM.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	_backGround = IMAGEMANAGER->FindImage(_tiles->backGroundName);

	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute == PLAYER)
		{
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}

		if (_tiles[i].attribute == OBJ)
		{

		}
	}

	CloseHandle(file);
}

void soundStageN::tileCollision()
{
	FloatRect pFrc = _player->getPlayerFrc();

	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (IntersectRectToRect(&pFrc, &_tiles[i*TILEX + j].rc) && _tiles[i*TILEX + j].isCollider)
			{
				switch (_player->getPdirec())
				{
				case CHRDIREC_DOWN:
					_player->setPLocaY(_tiles[i*TILEX + j].rc.top - TILESIZE / 4 * 3);
					break;
				case CHRDIREC_LEFT:
					_player->setPLocaX(_tiles[i*TILEX + j].rc.right + 4);
					break;
				case CHRDIREC_RIGHT:
					_player->setPLocaX(_tiles[i*TILEX + j].rc.left - TILESIZE / 4 * 3);
					break;
				case CHRDIREC_UP:
					_player->setPLocaY(_tiles[i*TILEX + j].rc.bottom + 4);
					break;
				}
			}
			
		}
	}
	if (IntersectRectToRect(&pFrc, &mapChange[0].rc))
	{
		SCENEMANAGER->changeScene("4층홀", CHRDIREC_DOWN);
	}
	if (IntersectRectToRect(&pFrc, &_tiles[DOOR].rc) && STAGEMEMORYMANAGER->getIsPiano() && STAGEMEMORYMANAGER->getIsOpen())
	{
		/////////////////////////다음스테이지 넣을곳///////////////////////////
		SCENEMANAGER->changeScene("garden_5f", CHRDIREC_UP, LOCATION_DEFAULT);
		/////////////////////////////////////////////////////////////////////
	}
}

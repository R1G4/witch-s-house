#include "stdafx.h"
#include "soundStageE2.h"

soundStageE2::soundStageE2()
{
}

soundStageE2::~soundStageE2()
{
}

HRESULT soundStageE2::init()
{

	return S_OK;
}

HRESULT soundStageE2::init(CHRDIRECTION _CHRDIRECTION)
{
	if (_CHRDIRECTION == CHRDIREC_UP)
	{

		_rcAlpha = 1.0f;
		IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/타일.png", 7, 2);
		IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
		IMAGEMANAGER->AddFrameImage("SavePoint", L"Image/mapTool/saveCat.png", 16, 4);


		CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

		_player = new Player;
		load();
		_player->init();
		_player->setDirec(CHRDIREC_UP);
		camera.x = _player->getPlayerLocX();
		camera.y = _player->getPlayerLocY();
		CAMERAMANAGER->setCamera(camera);

		_isStopToRead = false;
		_isClick = false;
		_disCover = false;
		
		mapChange[0].x = WINSIZEX / 2 - 378 + 48 * 15;
		mapChange[0].y = WINSIZEY / 2 - 120 + 48 * 12;

		mapChange[0].rc = RectMakeCenter(mapChange[0].x, mapChange[0].y, 120, 24);

	}

	return S_OK;
}

void soundStageE2::release()
{
}

void soundStageE2::update()
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
			if (IntersectRectToRect(&_tiles[PAPER].rc, &pSRc))
			{
				_vScript = TEXTMANAGER->loadFile("dialog/4f/4f_quiz.txt");
				_isStopToRead = true;
			}
			if (IntersectRectToRect(&_tiles[BOOKS].rc, &pSRc))
			{
				_isClick = true;
			}
			if (IntersectRectToRect(&_tiles[DOLL].rc, &pSRc))
			{
				_isClick = true;
			}
			if (IntersectRectToRect(&_tiles[CLOCK].rc, &pSRc))
			{
				_isClick = true;
			}
			if (IntersectRectToRect(&_tiles[PUMPKIN].rc, &pSRc))
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

		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			_disCover = true;
			_rc = RectMakePivot(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			_disCover = false;
			_rc = RectMakePivot(Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			_isClick = false;
			if (IntersectRectToRect(&_tiles[BOOKS].rc, &pSRc))
			{
				if (_disCover)
				{
					_vScript = TEXTMANAGER->loadFile("dialog/4f/4f_discover_fail.txt");
					_isStopToRead = true;
				}
			}
			if (IntersectRectToRect(&_tiles[DOLL].rc, &pSRc))
			{
				if (_disCover)
				{
					_vScript = TEXTMANAGER->loadFile("dialog/4f/4f_discover_fail.txt");
					_isStopToRead = true;
				}
			}
			if (IntersectRectToRect(&_tiles[PUMPKIN].rc, &pSRc))
			{
				if (_disCover)
				{
					_vScript = TEXTMANAGER->loadFile("dialog/4f/4f_discover_fail.txt");
					_isStopToRead = true;
				}
			}
			if (IntersectRectToRect(&_tiles[CLOCK].rc, &pSRc))
			{
				if (STAGEMEMORYMANAGER->getIsGetTae() && _disCover)
				{
					_vScript = TEXTMANAGER->loadFile("dialog/4f/4f_discover_fail.txt");
					_isStopToRead = true;
				}
				if (_disCover)
				{
					_vScript = TEXTMANAGER->loadFile("dialog/4f/4f_discover.txt");
					_isStopToRead = true;
					STAGEMEMORYMANAGER->setIsGetTae(true);
					
				}
				ITEMMANAGER->addItem("obj33");
				
			}
		}
	}

}

void soundStageE2::render()
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
			CAMERAMANAGER->render(ZORDER->getZorder()[i].img, Vector2(ZORDER->getZorder()[i].x + TILESIZE / 2, ZORDER->getZorder()[i].y - 12));
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
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 250, camera.y));
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x + 250, camera.y));

		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 300, 345, L"살펴본다", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 117, 345, L"아무것도 하지 않는다", RGB(255, 255, 255), 0.85f, 27);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5f);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
	}
}

void soundStageE2::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/4F_EROOM2.map", GENERIC_READ, NULL, NULL,
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

void soundStageE2::tileCollision()
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
		SCENEMANAGER->changeScene("4층오른쪽방1", CHRDIREC_DOWN);
	}
}

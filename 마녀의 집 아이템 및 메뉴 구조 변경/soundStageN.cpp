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
	IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/Å¸ÀÏ.png", 7, 2);
	IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
	IMAGEMANAGER->AddFrameImage("SavePoint", L"Image/mapTool/saveCat.png", 16, 4);
	IMAGEMANAGER->AddImage("´ÝÈùÇÇ¾Æ³ë", L"Image/piano-C.png");
	IMAGEMANAGER->AddImage("¿­¸°ÇÇ¾Æ³ë", L"Image/piano-O.png");



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
		IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/Å¸ÀÏ.png", 7, 2);
		IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
		IMAGEMANAGER->AddFrameImage("SavePoint", L"Image/mapTool/saveCat.png", 16, 4);
		IMAGEMANAGER->AddImage("´ÝÈùÇÇ¾Æ³ë", L"Image/piano-C.png");
		IMAGEMANAGER->AddImage("¿­¸°ÇÇ¾Æ³ë", L"Image/piano-O.png");



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
	}

	return S_OK;
}

void soundStageN::release()
{
}

void soundStageN::update()
{
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();

	CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));
	_player->update();
	tileCollision();
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
	ZORDER->insert(Px, Py, "´ÝÈùÇÇ¾Æ³ë", ZOBJECT);
	ZORDER->insert(_player->getPlayerLocX(), _player->getPlayerLocY(), ZPLAYER);

	for (int i = 0; i < ZORDER->getZorder().size(); i++)
	{
		if (ZORDER->getZorder()[i].type == ZOBJECT)
		{
			CAMERAMANAGER->render(ZORDER->getZorder()[i].img, Vector2(Px, Py));
		}
		if (ZORDER->getZorder()[i].type == ZPLAYER)
		{
			_player->render();
		}

	}
	ZORDER->release();

	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2));
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
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (IntersectRectToRect(&_player->getPlayerFrc(), &_tiles[i*TILEX + j].rc) && _tiles[i*TILEX + j].isCollider)
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
	if (IntersectRectToRect(&_player->getPlayerFrc(), &mapChange[0].rc))
	{
		SCENEMANAGER->changeScene("4ÃþÈ¦", CHRDIREC_DOWN);
	}
}

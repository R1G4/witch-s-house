#include "stdafx.h"
#include "soundStage.h"

soundStage::soundStage()
{
}

soundStage::~soundStage()
{
}

HRESULT soundStage::init()
{
	IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/타일.png", 7, 2);
	IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
	IMAGEMANAGER->AddFrameImage("SavePoint", L"Image/mapTool/saveCat.png", 16, 4);
	//IMAGEMANAGER->AddImage("병정", L"Image/obj/021.png");
	
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

	_player = new Player;
	load();
	_player->init();
	_player->setDirec(CHRDIREC_UP);
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	CAMERAMANAGER->setCamera(camera);
	frame = 0;

	mapChange[0].x = WINSIZEX / 2 + 343;
	mapChange[0].y = WINSIZEY / 2 - 100;

	mapChange[1].x = WINSIZEX / 2 + 343;
	mapChange[1].y = WINSIZEY / 2 + 522;

	mapChange[2].x = WINSIZEX / 2 + 315 - 48 * 4;
	mapChange[2].y = WINSIZEY / 2 + 504 - 48 * 4;

	mapChange[3].x = WINSIZEX / 2 + 320 + 48 * 5;
	mapChange[3].y = WINSIZEY / 2 + 504 - 48 * 4;

	mapChange[0].rc = RectMakeCenter(mapChange[0].x, mapChange[0].y, 24, 24);
	mapChange[1].rc = RectMakeCenter(mapChange[1].x, mapChange[1].y, 120, 24);
	mapChange[2].rc = RectMakeCenter(mapChange[2].x, mapChange[2].y, 24, 72);
	mapChange[3].rc = RectMakeCenter(mapChange[3].x, mapChange[3].y, 24, 72);

	return S_OK;
}

HRESULT soundStage::init(CHRDIRECTION _CHRDIRECTION)
{
	if (_CHRDIRECTION == CHRDIREC_RIGHT)
	{
		IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/타일.png", 7, 2);
		IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
		IMAGEMANAGER->AddFrameImage("SavePoint", L"Image/mapTool/saveCat.png", 16, 4);
		//IMAGEMANAGER->AddImage("병정", L"Image/obj/021.png");

		CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

		_player = new Player;
		load();
		_player->setStart((WINSIZEX / 2 + 405 - 48 * 4) / TILESIZE, (WINSIZEY / 2 + 504 - 48 * 4) / TILESIZE);
		_player->init();
		_player->setDirec(CHRDIREC_RIGHT);

		camera.x = _player->getPlayerLocX();
		camera.y = _player->getPlayerLocY();
		CAMERAMANAGER->setCamera(camera);
		frame = 0;

		mapChange[0].x = WINSIZEX / 2 + 343;
		mapChange[0].y = WINSIZEY / 2 - 100;

		mapChange[1].x = WINSIZEX / 2 + 343;
		mapChange[1].y = WINSIZEY / 2 + 522;

		mapChange[2].x = WINSIZEX / 2 + 315 - 48 * 4;
		mapChange[2].y = WINSIZEY / 2 + 504 - 48 * 4;

		mapChange[3].x = WINSIZEX / 2 + 320 + 48 * 5;
		mapChange[3].y = WINSIZEY / 2 + 504 - 48 * 4;

		mapChange[0].rc = RectMakeCenter(mapChange[0].x, mapChange[0].y, 24, 24);
		mapChange[1].rc = RectMakeCenter(mapChange[1].x, mapChange[1].y, 120, 24);
		mapChange[2].rc = RectMakeCenter(mapChange[2].x, mapChange[2].y, 24, 72);
		mapChange[3].rc = RectMakeCenter(mapChange[3].x, mapChange[3].y, 24, 72);

	
	}

	if (_CHRDIRECTION == CHRDIREC_LEFT)
	{
		IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/타일.png", 7, 2);
		IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
		IMAGEMANAGER->AddFrameImage("SavePoint", L"Image/mapTool/saveCat.png", 16, 4);
		//IMAGEMANAGER->AddImage("병정", L"Image/obj/021.png");

		CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

		_player = new Player;
		load();
		_player->setStart((WINSIZEX / 2 + 224 + 48 * 5) / TILESIZE, (WINSIZEY / 2 + 504 - 48 * 4) / TILESIZE);
		_player->init();
		_player->setDirec(CHRDIREC_LEFT);

		camera.x = _player->getPlayerLocX();
		camera.y = _player->getPlayerLocY();
		CAMERAMANAGER->setCamera(camera);
		frame = 0;

		mapChange[0].x = WINSIZEX / 2 + 343;
		mapChange[0].y = WINSIZEY / 2 - 100;

		mapChange[1].x = WINSIZEX / 2 + 343;
		mapChange[1].y = WINSIZEY / 2 + 522;

		mapChange[2].x = WINSIZEX / 2 + 315 - 48 * 4;
		mapChange[2].y = WINSIZEY / 2 + 504 - 48 * 4;

		mapChange[3].x = WINSIZEX / 2 + 320 + 48 * 5;
		mapChange[3].y = WINSIZEY / 2 + 504 - 48 * 4;

		mapChange[0].rc = RectMakeCenter(mapChange[0].x, mapChange[0].y, 24, 24);
		mapChange[1].rc = RectMakeCenter(mapChange[1].x, mapChange[1].y, 120, 24);
		mapChange[2].rc = RectMakeCenter(mapChange[2].x, mapChange[2].y, 24, 72);
		mapChange[3].rc = RectMakeCenter(mapChange[3].x, mapChange[3].y, 24, 72);

	
	}
	if (_CHRDIRECTION == CHRDIREC_DOWN)
	{
		IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/타일.png", 7, 2);
		IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
		IMAGEMANAGER->AddFrameImage("SavePoint", L"Image/mapTool/saveCat.png", 16, 4);
		//IMAGEMANAGER->AddImage("병정", L"Image/obj/021.png");

		CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

		_player = new Player;
		load();
		_player->setStart((WINSIZEX / 2 + 224 + 48 * 2) / TILESIZE, (WINSIZEY / 2 + 504 - 48 * 11) / TILESIZE);
		_player->init();
		_player->setDirec(CHRDIREC_DOWN);

		camera.x = _player->getPlayerLocX();
		camera.y = _player->getPlayerLocY();
		CAMERAMANAGER->setCamera(camera);
		frame = 0;

		mapChange[0].x = WINSIZEX / 2 + 343;
		mapChange[0].y = WINSIZEY / 2 - 100;

		mapChange[1].x = WINSIZEX / 2 + 343;
		mapChange[1].y = WINSIZEY / 2 + 522;

		mapChange[2].x = WINSIZEX / 2 + 315 - 48 * 4;
		mapChange[2].y = WINSIZEY / 2 + 504 - 48 * 4;

		mapChange[3].x = WINSIZEX / 2 + 320 + 48 * 5;
		mapChange[3].y = WINSIZEY / 2 + 504 - 48 * 4;

		mapChange[0].rc = RectMakeCenter(mapChange[0].x, mapChange[0].y, 24, 24);
		mapChange[1].rc = RectMakeCenter(mapChange[1].x, mapChange[1].y, 120, 24);
		mapChange[2].rc = RectMakeCenter(mapChange[2].x, mapChange[2].y, 24, 72);
		mapChange[3].rc = RectMakeCenter(mapChange[3].x, mapChange[3].y, 24, 72);

	}
	return S_OK;
}

void soundStage::update()
{
	count++;
	if (count % 4 == 0)
	{
		frame++;
		if (frame > 15)
		{
			frame = 0;
		}
	}
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();

	CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));
	_player->update();
	tileCollision();
}

void soundStage::release()
{
}

void soundStage::render()
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
				CAMERAMANAGER->renderRc(mapChange[1].rc, D2D1::ColorF::White, 1.0f, 24);
				CAMERAMANAGER->renderRc(mapChange[2].rc, D2D1::ColorF::White, 1.0f, 24);
				CAMERAMANAGER->renderRc(mapChange[3].rc, D2D1::ColorF::White, 1.0f, 24);
			}

		}
	}
	CAMERAMANAGER->FrameRender(IMAGEMANAGER->FindImage("SavePoint"), Vector2(1080, 553), frame, 1);
	//CAMERAMANAGER->render(IMAGEMANAGER->FindImage("병정"), Vector2(1080, 553));
	_player->render();
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



void soundStage::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/4f_hall.map", GENERIC_READ, NULL, NULL,
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

void soundStage::tileCollision()
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
		SCENEMANAGER->changeScene("4층윗방", CHRDIREC_UP);
	}
	if (IntersectRectToRect(&pFrc, &mapChange[1].rc))
	{
		
	}
	if (IntersectRectToRect(&pFrc, &mapChange[2].rc))
	{
		SCENEMANAGER->changeScene("4층왼쪽방");
	}
	if (IntersectRectToRect(&pFrc, &mapChange[3].rc))
	{
		SCENEMANAGER->changeScene("4층오른쪽방1", CHRDIREC_RIGHT);
	}
}

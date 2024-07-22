#include "stdafx.h"
#include "garDen.h"

garDen::garDen()
{
}

garDen::~garDen()
{
}

HRESULT garDen::init()
{
	IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/타일.png", 7, 2);
	IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
	IMAGEMANAGER->AddFrameImage("SavePoint", L"Image/mapTool/saveCat.png", 16, 4);
	IMAGEMANAGER->AddImage("정원나무", L"Image/mapTool/tileset/001_tr.png");
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);
	
	_player = new Player;
	load();
	_player->init();
	_player->setDirec(CHRDIREC_DOWN);
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	CAMERAMANAGER->setCamera(camera);
	frame = 0;

	mapChange[0].x = WINSIZEX / 2 + 775;
	mapChange[0].y = WINSIZEY / 2 - 140;

	mapChange[1].x = WINSIZEX / 2 + 391;
	mapChange[1].y = WINSIZEY / 2 + 790;

	mapChange[0].rc = RectMakeCenter(mapChange[0].x, mapChange[0].y, 24, 24);
	mapChange[1].rc = RectMakeCenter(mapChange[1].x, mapChange[1].y, 24, 24);
	
	return S_OK;
}

HRESULT garDen::init(CHRDIRECTION _CHRDIRECTION)
{
	if (_CHRDIRECTION == CHRDIREC_UP)
	{
		_player = new Player;
		load();
		_player->setStart((WINSIZEX / 2 + 391) / TILESIZE, (WINSIZEY / 2 + 650) / TILESIZE);
		_player->init();
		_player->setDirec(CHRDIREC_UP);
		camera.x = _player->getPlayerLocX();
		camera.y = _player->getPlayerLocY();
		CAMERAMANAGER->setCamera(camera);
		frame = 0;

		mapChange[0].x = WINSIZEX / 2 + 775;
		mapChange[0].y = WINSIZEY / 2 - 140;

		mapChange[1].x = WINSIZEX / 2 + 391;
		mapChange[1].y = WINSIZEY / 2 + 790;

		mapChange[0].rc = RectMakeCenter(mapChange[0].x, mapChange[0].y, 24, 24);
		mapChange[1].rc = RectMakeCenter(mapChange[1].x, mapChange[1].y, 24, 24);
	}
	if (_CHRDIRECTION == CHRDIREC_DOWN)
	{
		_player = new Player;
		load();
		_player->setStart((WINSIZEX / 2 + 775) / TILESIZE, (WINSIZEY / 2) / TILESIZE);
		_player->init();
		_player->setDirec(CHRDIREC_DOWN);
		camera.x = _player->getPlayerLocX();
		camera.y = _player->getPlayerLocY();
		CAMERAMANAGER->setCamera(camera);
		frame = 0;

		mapChange[0].x = WINSIZEX / 2 + 775;
		mapChange[0].y = WINSIZEY / 2 - 140;

		mapChange[1].x = WINSIZEX / 2 + 391;
		mapChange[1].y = WINSIZEY / 2 + 790;

		mapChange[0].rc = RectMakeCenter(mapChange[0].x, mapChange[0].y, 24, 24);
		mapChange[1].rc = RectMakeCenter(mapChange[1].x, mapChange[1].y, 24, 24);
	}
	
		
	return S_OK;
}

void garDen::update()
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

void garDen::release()
{
}

void garDen::render()
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
			}

		}
	}
	CAMERAMANAGER->FrameRender(IMAGEMANAGER->FindImage("SavePoint"), Vector2(1035, 755), frame, 2);
	
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
	
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("정원나무"), Vector2(720 + 480, 648));
}



void garDen::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/gardenscene.map", GENERIC_READ, NULL, NULL,
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

void garDen::tileCollision()
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
		SCENEMANAGER->changeScene("성앞", CHRDIREC_UP);
	}
	if (IntersectRectToRect(&pFrc, &mapChange[1].rc))
	{
		SCENEMANAGER->changeScene("정원아래", CHRDIREC_DOWN);
	}
}

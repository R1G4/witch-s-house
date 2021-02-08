#include "stdafx.h"
#include "garDenUnder.h"

garDenUnder::garDenUnder()
{
}

garDenUnder::~garDenUnder()
{
}

HRESULT garDenUnder::init(CHRDIRECTION _chrdirection)
{
	IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/타일.png", 7, 2);
	IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
	IMAGEMANAGER->AddImage("장미", L"Image/mapTool/tileset/002_tr.png");
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);
	if (_chrdirection == CHRDIREC_DOWN)
	{
		_player = new Player;
		load();
		_player->init();
		_player->setDirec(CHRDIREC_DOWN);
		camera.x = _player->getPlayerLocX();
		camera.y = _player->getPlayerLocY();
		CAMERAMANAGER->setCamera(camera);

		mapChange.x = WINSIZEX / 2 + 775 - 96;
		mapChange.y = WINSIZEY / 2 - 140 - 48;

		mapChange.rc = RectMakeCenter(mapChange.x, mapChange.y, 24, 24);
	}
	

	return S_OK;
}

void garDenUnder::update()
{
	
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();

	CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));
	_player->update();
	tileCollision();
}

void garDenUnder::release()
{
}

void garDenUnder::render()
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
				CAMERAMANAGER->renderRc(mapChange.rc, D2D1::ColorF::White, 1.0f, 24);
			}

		}
	}
	
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

	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("장미"), Vector2(_backGround->GetSize().x / 2 + 480, _backGround->GetSize().y / 2));
}



void garDenUnder::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/gardenscene2.map", GENERIC_READ, NULL, NULL,
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

void garDenUnder::tileCollision()
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
	if (IntersectRectToRect(&_player->getPlayerFrc(), &mapChange.rc))
	{
		SCENEMANAGER->changeScene("정원", CHRDIREC_UP);
	}
}

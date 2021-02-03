#include "stdafx.h"
#include "third2.h"

third2::third2()
{
}

third2::~third2()
{
}

HRESULT third2::init()
{
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

	_player = new Player;
	load();

	_player->init();
	_player->setState(CHR_IDLE);
	_player->setDirec(CHRDIREC_DOWN);
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	CAMERAMANAGER->setCamera(camera);
	return S_OK;
}

void third2::release()
{
}

void third2::update()
{
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();

	CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));
	_player->update();
	tileCollision();
}

void third2::render()
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
			}

		}
	}
	_player->render();

	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;
			//IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->SetAlpha(0.5);
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top));
		}
	}
}

void third2::trigger()
{
}

void third2::tileCollision()
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
			if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)
			{
				if (IntersectRectToRect(&_player->getPlayerFrc(), &_tiles[i*TILEX + j].rc))
				{
					//SCENEMANAGER->changeScene();
				}
			}

		}
	}
}

void third2::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/3f_2.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	_backGround = IMAGEMANAGER->FindImage(_tiles->backGroundName);
	cout << _tiles->camera.x;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute == PLAYER)
		{
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}
		/*if (_tiles[i].attribute == ENEMY)
		{
			bossLocX = i % TILEX;
			bossLocY = i / TILEX;
		}*/
		if (_tiles[i].attribute == OBJ)
		{

		}
	}

	CloseHandle(file);
}
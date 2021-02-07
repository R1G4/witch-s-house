#include "stdafx.h"
#include "soundStageD.h"

soundStageD::soundStageD()
{
}

soundStageD::~soundStageD()
{
}

HRESULT soundStageD::init()
{
	IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/Å¸ÀÏ.png", 7, 2);
	IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
	IMAGEMANAGER->AddImage("º´Á¤", L"Image/obj/022.png");
	IMAGEMANAGER->AddFrameImage("»ç¸Á", L"Image/4F_Dead.png", 8, 1);

	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

	_player = new Player;
	load();
	_player->setStart((WINSIZEX / 2 + 405 - 48 * 4) / TILESIZE, (WINSIZEY / 2 + 504 - 48 * 4) / TILESIZE);
	_player->init();
	_player->setDirec(CHRDIREC_RIGHT);

	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	CAMERAMANAGER->setCamera(camera);
	frame = 0.5f;
	aniFrame = 0;
	_player->update();

	
	return S_OK;
}

HRESULT soundStageD::init(CHRDIRECTION _CHRDIRECTION)
{
	if (_CHRDIRECTION == CHRDIREC_RIGHT)
	{
		IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/Å¸ÀÏ.png", 7, 2);
		IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
		IMAGEMANAGER->AddImage("º´Á¤", L"Image/obj/022.png");
		IMAGEMANAGER->AddFrameImage("»ç¸Á", L"Image/4F_Dead.png", 8, 1);

		CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

		_player = new Player;
		load();
		_player->setStart((WINSIZEX / 2 + 405 - 48 * 4) / TILESIZE, (WINSIZEY / 2 + 504 - 48 * 4) / TILESIZE);
		_player->init();
		_player->setDirec(CHRDIREC_RIGHT);

		camera.x = _player->getPlayerLocX();
		camera.y = _player->getPlayerLocY();
		CAMERAMANAGER->setCamera(camera);
		frame = 0.5f;
		aniFrame = 0;
	

		_player->update();
	}

	
	return S_OK;
}

void soundStageD::update()
{
	count++;
	if (count % 9 == 0)
	{
		aniFrame++;
		if (aniFrame > 7)
		{
			aniFrame = 7;
		}
	}
	
	frame -= 0.05f;
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	
	CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));
	tileCollision();
}

void soundStageD::release()
{
}

void soundStageD::render()
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
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("º´Á¤"), Vector2(980, 683));
	CAMERAMANAGER->FrameRender(IMAGEMANAGER->FindImage("»ç¸Á"), Vector2(_player->getPlayerLocX()+24, _player->getPlayerLocY()-15), aniFrame, 0);
	//_player->render();
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2));
		}
	}
	CAMERAMANAGER->renderRc(effect, D2D1::ColorF::Red, frame, 10000000);

}



void soundStageD::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/4f_hallD.map", GENERIC_READ, NULL, NULL,
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

void soundStageD::tileCollision()
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
	
}

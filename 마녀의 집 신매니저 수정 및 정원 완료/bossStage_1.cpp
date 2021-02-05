#include "stdafx.h"
#include "bossStage_1.h"

bossStage_1::bossStage_1()
{
}

bossStage_1::~bossStage_1()
{
}

HRESULT bossStage_1::init()
{
	load();
	bossStage::init();
	_player->setDirec(CHRDIREC_UP);

	return S_OK;
}

void bossStage_1::release()
{
	bossStage::release();
	_vTrigger.clear();
}

void bossStage_1::update()
{
	bossStage::update();
	activeCorr();
	activeTrigger();
}

void bossStage_1::render()
{
	bossStage::render();
}


void bossStage_1::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/ElenRoom.map", GENERIC_READ, NULL, NULL,
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
		if (_tiles[i].attribute == ENEMY)
		{
			bossLocX = i % TILEX;
			bossLocY = i / TILEX;
		}
	}

	CloseHandle(file);
}

void bossStage_1::activeTrigger()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (IntersectRectToRect(&_player->getSearchRc(), &_tiles[i*TILEX + j].rc) && _tiles[i*TILEX + j].terrain == TR_TRIGGER && !_isBossAppeal)
			{
				if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
				{
					_boss->init(bossLocX, bossLocY);
					_isBossAppeal = true;
				}
			}
		}
	}

}

void bossStage_1::activeCorr()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (IntersectRectToRect(&_player->getPlayerFrc(), &_tiles[i*TILEX + j].rc) && _tiles[i*TILEX + j].obj == OBJ_CORELATION)
			{
				_Stop = true;
				alpha -= 0.01;
				_player->setAlpha(alpha);
				_boss->setalpha(alpha);
				if (alpha <= 0)
				{
					SCENEMANAGER->changeScene("BossStage2");
					_boss->setalpha(1);
					_player->setAlpha(1);
				}
			}
		}
	}
}

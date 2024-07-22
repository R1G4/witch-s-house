#include "stdafx.h"
#include "bossStage_5.h"

bossStage_5::bossStage_5()
{
}

bossStage_5::~bossStage_5()
{
}

HRESULT bossStage_5::init()
{
	load();
	bossStage::init();
	_player->setDirec(CHRDIREC_RIGHT);
	return S_OK;
}

void bossStage_5::release()
{
	bossStage::init();
}

void bossStage_5::update()
{
	bossStage::update();
	activeCorr();
	activeTrigger();
}

void bossStage_5::render()
{
	bossStage::render();
}

void bossStage_5::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/finalEntrance.map", GENERIC_READ, NULL, NULL,
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
		if (_tiles[i].attribute == ENEMY)
		{
			bossLocX = i % TILEX;
			bossLocY = i / TILEX;
		}
	}

	CloseHandle(file);
}

void bossStage_5::activeTrigger()
{

	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			FloatRect pSRc = _player->getSearchRc();
			if (IntersectRectToRect(&pSRc, &_tiles[i*TILEX + j].rc) && _tiles[i*TILEX + j].terrain == TR_TRIGGER && !_isBossAppeal)
			{
					_boss->init(bossLocX, bossLocY);
					_isBossAppeal = true;
			}
		}
	}
}

void bossStage_5::activeCorr()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			FloatRect pFrc = _player->getPlayerFrc();
			if (IntersectRectToRect(&pFrc, &_tiles[i*TILEX + j].rc) && _tiles[i*TILEX + j].obj == OBJ_CORELATION)
			{
				_Stop = true;
				alpha -= 0.01;
				_player->setAlpha(alpha);
				_boss->setalpha(alpha);
				if (alpha <= 0)
				{
					SCENEMANAGER->changeScene("Ending");
					_boss->setalpha(1);
					_player->setAlpha(1);
				}
			}
		}
	}
}

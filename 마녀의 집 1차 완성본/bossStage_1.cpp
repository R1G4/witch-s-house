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

HRESULT bossStage_1::init(int x, int y, CHRDIRECTION direc)
{
	SOUNDMANAGER->play("Boss");

	load();
	bossStage::init();
	_player->setPLocaX(x);
	_player->setPLocaY(y);
	_player->setDirec(direc);
	_boss->init(bossLocX, bossLocY);
	_isBossAppeal = true;
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
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute == PLAYER)
		{
			_player->setStart(i%TILEX, i / TILEX);
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
			FloatRect pSRc = _player->getSearchRc();
			if (IntersectRectToRect(&pSRc, &_tiles[i*TILEX + j].rc) && _tiles[i*TILEX + j].terrain == TR_TRIGGER)
			{
				STORAGEMANAGER->setStage(FINALSTAGE);
				save_x = _player->getPlayerLocX();
				save_y = _player->getPlayerLocY();
				save_direc = _player->getPdirec();
				save_s_x = to_string(save_x);
				save_s_y = to_string(save_y);
				save_s_direc = to_string(save_direc);
				INIDATA->addData("비올라", "x좌표", save_s_x.c_str());
				INIDATA->addData("비올라", "y좌표", save_s_y.c_str());
				INIDATA->addData("비올라", "방향", save_s_direc.c_str());
				if (STORAGEMANAGER->saveView()&&!_isBossAppeal)
				{
					_Stop = true;
					SOUNDMANAGER->stop("BossStage");
					SOUNDMANAGER->play("Boss");
					_boss->init(bossLocX, bossLocY);
					_isBossAppeal = true;
				}
				else _Stop = false;
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
			FloatRect pFrc = _player->getPlayerFrc();
			if (IntersectRectToRect(&pFrc, &_tiles[i*TILEX + j].rc) && _tiles[i*TILEX + j].obj == OBJ_CORELATION)
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

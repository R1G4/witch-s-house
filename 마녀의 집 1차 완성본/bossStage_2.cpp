#include "stdafx.h"
#include "bossStage_2.h"

bossStage_2::bossStage_2()
{
}

bossStage_2::~bossStage_2()
{
}

HRESULT bossStage_2::init()
{
	load();
	bossStage::init();
	_player->setDirec(CHRDIREC_DOWN);
	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	return S_OK;
}

void bossStage_2::release()
{
	bossStage::release();
	_vTrigger.clear();
}

void bossStage_2::update()
{
	bossStage::update();
	activeTrigger();
	activeCorr();
}

void bossStage_2::render()
{
	_backGround->SetAlpha(alpha);
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
			ZORDER->insert(_player->getPlayerLocX(), _player->getPlayerLocY(), ZPLAYER);
			if (_isBossAppeal)ZORDER->insert(_boss->getRect().left, _boss->getRect().top, ZENEMY);
			for (int i = 0; i < TILEX*TILEY; i++)
			{
				if (_tiles[i].obj != OBJ_NONE)
				{
					if (_tiles[i].keyName == "obj5" || _tiles[i].keyName == "obj9")continue;
					ZORDER->insert(_tiles[i].rc.left, _tiles[i].rc.bottom,_tiles[i].keyName, ZOBJECT);
				}
			}
			for (int i = 0; i < _vFrameTile.size(); i++)
			{
				ZORDER->insert(_vFrameTile[i].rc.left, _vFrameTile[i].rc.top, _vFrameTile[i].keyName, ZFRAMEOBJ);
			}
			for (int i = 0; i < ZORDER->getZorder().size(); i++)
			{

				if (ZORDER->getZorder()[i].type == ZPLAYER)_player->render();

				if (ZORDER->getZorder()[i].type == ZENEMY)_boss->render();

				if (ZORDER->getZorder()[i].type == ZOBJECT)
				{
					if ((ZORDER->getZorder()[i].keyName == "obj61" && !_vTrigger[2].isTriggerOn) && (ZORDER->getZorder()[i].keyName == "obj61" && !_vTrigger[5].isTriggerOn))continue;
					if ((ZORDER->getZorder()[i].keyName == "obj59" && !_vTrigger[0].isTriggerOn) && (ZORDER->getZorder()[i].keyName == "obj59" && !_vTrigger[3].isTriggerOn))continue;
					CAMERAMANAGER->render(ZORDER->getZorder()[i].img,
						Vector2(ZORDER->getZorder()[i].x + TILESIZE / 2, ZORDER->getZorder()[i].y  - ZORDER->getZorder()[i].img->GetSize().y / 2));
				}
				if (ZORDER->getZorder()[i].type == ZFRAMEOBJ)
				{
					for (int j = 0; j < _vFrameTile.size(); j++)
					{
						if (_vFrameTile[j].kinds == PLAYER) continue;
						if (_vFrameTile[j].kinds == ENEMY)continue;
						if (ZORDER->getZorder()[i].keyName == _vFrameTile[j].keyName)
							CAMERAMANAGER->FrameRender
							(
								_vFrameTile[j].img,
								Vector2((_vFrameTile[j].rc.left + _vFrameTile[j].rc.right) / 2, _vFrameTile[j].rc.bottom - _vFrameTile[j].img->GetSize().y / 2),
								_vFrameTile[j].frameX, _vFrameTile[j].frameY
							);
					}
				}
			}
			ZORDER->release();
			for (int i = 0; i < TILEX*TILEY; i++)
			{
				if (_tiles[i].keyName == "obj5" || _tiles[i].keyName == "obj9")
					CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i].keyName),
						Vector2(_tiles[i].rc.left + TILESIZE / 2, _tiles[i].rc.bottom - IMAGEMANAGER->FindImage(_tiles[i].keyName)->GetSize().y / 2));
			}
			IMAGEMANAGER->FindImage("Back2")->SetAlpha(1.0f);
			IMAGEMANAGER->FindImage("Back2")->SetSize(Vector2(1920, 1280));
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage("Back2"), Vector2(_player->getPlayerLocX(), _player->getPlayerLocY()));
	dead->render();
}

void bossStage_2::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/finalKitchen.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	_backGround = IMAGEMANAGER->FindImage(_tiles->backGroundName);
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute == PLAYER)
		{
			_player->setStart(i%TILEX, i / TILEX);
			continue;
		}
		if (_tiles[i].attribute == ENEMY)
		{
			bossLocX = i % TILEX;
			bossLocY = i / TILEX;
		}

		if (_tiles[i].terrain == TR_TRIGGER)
		{
			Trigger _triggerTile;
			_triggerTile.tile = _tiles[i].rc;
			_triggerTile.idx = i % TILEX;
			_triggerTile.idy = i / TILEY;
			_triggerTile.isTriggerOn = false;
			_vTrigger.push_back(_triggerTile);
		}
	}
	CloseHandle(file);

}

void bossStage_2::activeTrigger()
{
	for (int i = 0; i < _vTrigger.size(); i++)
	{
		FloatRect temp;
		FloatRect pFrc = _player->getPlayerFrc();
		if (IntersectRectToRect(&temp, &pFrc, &_vTrigger[i].tile))
		{
			_vTrigger[i].isTriggerOn = true;
		}
	}
	FloatRect pFrc = _player->getPlayerFrc();
	if ((IntersectRectToRect(&pFrc, &_vTrigger[1].tile) || IntersectRectToRect(&pFrc, &_vTrigger[4].tile))&&!_isBossAppeal)
	{
		_boss->init(bossLocX, bossLocY);
		_isBossAppeal = true;
	}
}

void bossStage_2::activeCorr()
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
					SCENEMANAGER->changeScene("BossStage3");
					_boss->setalpha(1);
					_player->setAlpha(1);
				}
			}
		}
	}
}



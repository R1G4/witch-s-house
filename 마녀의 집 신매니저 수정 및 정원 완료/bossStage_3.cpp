#include "stdafx.h"
#include "bossStage_3.h"

bossStage_3::bossStage_3()
{
}

bossStage_3::~bossStage_3()
{
}

HRESULT bossStage_3::init()
{
	load();
	bossStage::init();
	_player->setDirec(CHRDIREC_DOWN);
	return S_OK;
}

void bossStage_3::release()
{
	bossStage::release();
	_vTrigger.clear();
}

void bossStage_3::update()
{
	bossStage::update();
	activeTrigger();
	activeCorr();
}

void bossStage_3::render()
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
	_player->render();
	if (_isBossAppeal)_boss->render();
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;
			//IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->SetAlpha(0.5);
			if ((_tiles[i*TILEX + j].keyName == "obj61" && !_vTrigger[5].isTriggerOn) && (_tiles[i*TILEX + j].keyName == "obj61" && !_vTrigger[2].isTriggerOn))continue;
			if ((_tiles[i*TILEX + j].keyName == "obj59" && !_vTrigger[2].isTriggerOn) && (_tiles[i*TILEX + j].keyName == "obj59" && !_vTrigger[1].isTriggerOn))continue;
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2,
					_tiles[i*TILEX + j].rc.bottom - IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->GetSize().y / 2));

		}
	}
	//타일에 프레임 이미지 배치 랜더
	for (int i = 0; i < _vFrameTile.size(); i++)
	{
		if (KEYMANAGER->isToggleKey(VK_TAB))
		{
			IMAGEMANAGER->FindImage(_vFrameTile[i].keyName)->SetAlpha(alpha);
			if (_vFrameTile[i].kinds == PLAYER)  CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::Blue, 0.7);
			else if (_vFrameTile[i].kinds == ENEMY)  CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::Black, 0.7);
			else CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::White, 0.7);
		}

		if (_vFrameTile[i].kinds == PLAYER) continue;
		if (_vFrameTile[i].kinds == ENEMY)continue;
		_vFrameTile[i].img->SetAlpha(alpha);
		CAMERAMANAGER->FrameRender
		(
			_vFrameTile[i].img,
			Vector2((_vFrameTile[i].rc.left + _vFrameTile[i].rc.right) / 2, _vFrameTile[i].rc.bottom - _vFrameTile[i].img->GetSize().y / 2),
			_vFrameTile[i].frameX, _vFrameTile[i].frameY
		);
	}

	dead->render();

}

void bossStage_3::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/finaldiningRoom.map", GENERIC_READ, NULL, NULL,
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
			continue;
		}
		if (_tiles[i].attribute == ENEMY)
		{
			bossLocX = i % TILEX;
			bossLocY = i / TILEX;
		}
		if (_tiles[i].terrain == TR_TRIGGER)
		{
			cout << "dd";
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

void bossStage_3::activeTrigger()
{
	for (int i = 0; i < _vTrigger.size(); i++)
	{
		FloatRect temp;
		if (IntersectRectToRect(&temp, &_player->getPlayerFrc(), &_vTrigger[i].tile))
		{
			cout << i << endl;
			_vTrigger[i].isTriggerOn = true;
		}
	}
	if ((IntersectRectToRect(&_player->getPlayerFrc(), &_vTrigger[3].tile) || IntersectRectToRect(&_player->getPlayerFrc(), &_vTrigger[7].tile)
		|| IntersectRectToRect(&_player->getPlayerFrc(), &_vTrigger[8].tile)|| IntersectRectToRect(&_player->getPlayerFrc(), &_vTrigger[9].tile))
		&& !_isBossAppeal)
	{
		_boss->init(bossLocX, bossLocY);
		_isBossAppeal = true;
	}

}

void bossStage_3::activeCorr()
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
					SCENEMANAGER->changeScene("BossStage4");
					_boss->setalpha(1);
					_player->setAlpha(1);
				}
			}
		}
	}

}

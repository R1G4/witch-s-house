#include "stdafx.h"
#include "prison_5f_well.h"
#include "Player.h"

HRESULT prison_5f_well::init()
{
	_player->setDirec(CHRDIREC_UP);

	//타일 불러오기
	load();

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	fifthFloorStage::init();

	return S_OK;
}

void prison_5f_well::release()
{
}

void prison_5f_well::update()
{
	fifthFloorStage::update();
	//_player->update();
	setFrameIndex();

	//카메라 관련 업데이트
	cameraUpdate();
	setTrigger();
	//cout << "x : " << (int)(_player->getPlayerLocX()) / TILESIZE << " y : " << (int)(_player->getPlayerLocY()) / TILESIZE << endl;
}

void prison_5f_well::render()
{	
	//배경 랜더
	IMAGEMANAGER->FindImage("배경68")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경68"),
		Vector2(IMAGEMANAGER->FindImage("배경68")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("배경68")->GetSize().y / 2));

	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			//if (!_tiles[i*TILEX + j].isMapOn)continue;
			if (KEYMANAGER->isToggleKey(VK_TAB))
			{
				CAMERAMANAGER->renderRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::White, 1, 1);
				if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aqua, 0.5);
				if (_tiles[i*TILEX + j].isCollider == true)CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Red, 0.5);
			}
		}
	}

	//타일에 프레임 이미지 배치 랜더
	for (int i = 0; i < _vFrameTile.size(); i++)
	{
		if (KEYMANAGER->isToggleKey(VK_TAB))
		{
			IMAGEMANAGER->FindImage(_vFrameTile[i].keyName)->SetAlpha(_sceneAlpha);
			if (_vFrameTile[i].kinds == PLAYER)  CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::Blue, 0.7);
			else if (_vFrameTile[i].kinds == ENEMY)  CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::Black, 0.7);
			else CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::White, 0.7);
		}

		if (_vFrameTile[i].kinds == PLAYER) continue;

		_vFrameTile[i].img->SetAlpha(0.5);
		CAMERAMANAGER->FrameRender
		(
			_vFrameTile[i].img,
			Vector2((_vFrameTile[i].rc.left + _vFrameTile[i].rc.right) / 2, _vFrameTile[i].rc.bottom - _vFrameTile[i].img->GetSize().y / 2),
			_vFrameTile[i].frameX, _vFrameTile[i].frameY
		);
	}

	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;

			IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->SetAlpha(_sceneAlpha);

			//중간에 배치하고 싶다면 이걸쓰세요. 디폴트 센타
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2,
					_tiles[i*TILEX + j].rc.bottom - IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->GetSize().y / 2));
		}
	}

	_player->render();
}

void prison_5f_well::Collision()
{
}

void prison_5f_well::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/5f_prison_well.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute == PLAYER)
		{
			_player->setStart(i % TILEX, i / TILEX);

			break;
		}
	}
	CloseHandle(file);
}

void prison_5f_well::setTrigger()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// 클릭행동 트리거
	{
		if (IntersectRectToRect(&_tiles[WATER].rc, &_player->getSearchRc()))
		{
			cout << "물!" << endl;
		}
		if (IntersectRectToRect(&_tiles[FLOG].rc, &_player->getSearchRc()))
		{
			cout << "올챙이!" << endl;
		}
	}
	if (IntersectRectToRect(&_tiles[DOORTOPRISON].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISON + 1].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISON - 1].rc, &_player->getPlayerFrc()))
	{
		sceneChange("prison_5f");
		cout << "감옥으로!" << endl;
	}
}

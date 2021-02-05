#include "stdafx.h"
#include "prison_5f.h"
#include "Player.h"

HRESULT prison_5f::init()
{
	_player->setDirec(CHRDIREC_RIGHT);

	//Ÿ�� �ҷ�����
	load();

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	fifthFloorStage::init();

	return S_OK;
}

void prison_5f::release()
{
	_player->release();
	_vFrameTile.clear();
}

void prison_5f::update()
{
	fifthFloorStage::update();
	//_player->update();
	setFrameIndex();

	//ī�޶� ���� ������Ʈ
	cameraUpdate();

	setTrigger();
	//cout << "x : " << (int)(_player->getPlayerLocX()) / TILESIZE << " y : " << (int)(_player->getPlayerLocY()) / TILESIZE << endl;
}

void prison_5f::render()
{	
	//��� ����
	IMAGEMANAGER->FindImage("���65")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("���65"),
		Vector2(IMAGEMANAGER->FindImage("���65")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("���65")->GetSize().y / 2));

	fifthFloorStage::render();
}

void prison_5f::Collision()
{
}

void prison_5f::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/5f_prison.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute == PLAYER)
		{
			_player->setStart(i % TILEX + 1, i / TILEX);

			break;
		}
	}
	CloseHandle(file);
}

void prison_5f::setTrigger()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// Ŭ���ൿ Ʈ����
	{
		if (IntersectRectToRect(&_tiles[R_FLOWER_1].rc, &_player->getSearchRc()))
		{
			cout << "��1!" << endl;
		}
		if (IntersectRectToRect(&_tiles[R_FLOWER_2].rc, &_player->getSearchRc()))
		{
			cout << "��2!" << endl;
		}
		if (IntersectRectToRect(&_tiles[R_FLOWER_3].rc, &_player->getSearchRc()))
		{
			cout << "��3!" << endl;
		}
		if (IntersectRectToRect(&_tiles[BOOK].rc, &_player->getSearchRc()))
		{
			cout << "å!" << endl;
		}
		if (IntersectRectToRect(&_tiles[PAPER].rc, &_player->getSearchRc()))
		{
			cout << "��Ʈ!" << endl;
		}
		if (IntersectRectToRect(&_tiles[CAGE].rc, &_player->getSearchRc()))
		{
			cout << "����!" << endl;
		}
	}

	if (IntersectRectToRect(&_tiles[SKUL].rc, &_player->getPlayerFrc()))
	{
		cout << "�ذ�!" << endl;
	}
	if (IntersectRectToRect(&_tiles[DOORTOGARDEN].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOGARDEN + TILEX].rc, &_player->getPlayerFrc()))
	{
		sceneChange("garden_5f");
		cout << "��������!" << endl;
	}
	if (IntersectRectToRect(&_tiles[DOORTOPRISONWELL].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISONWELL + 1].rc, &_player->getPlayerFrc()))
	{
		sceneChange("prison_well_5f");
		cout << "�칰��!" << endl;
	}
	if (IntersectRectToRect(&_tiles[DOORTOPRISONSKUL].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISONSKUL + TILEX].rc, &_player->getPlayerFrc()))
	{
		sceneChange("prison_skul_5f");
		cout << "�ذ������!" << endl;
	}
}

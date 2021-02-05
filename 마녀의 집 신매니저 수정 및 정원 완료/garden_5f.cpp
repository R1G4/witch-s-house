#include "stdafx.h"
#include "garden_5f.h"
#include "player.h"

HRESULT garden_5f::init()
{
	_player->setDirec(CHRDIREC_UP);

	//Ÿ�� �ҷ�����
	load();

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	fifthFloorStage::init();

	return S_OK;
}

void garden_5f::release()
{
}

void garden_5f::update()
{
	fifthFloorStage::update();
	//_player->update();
	setFrameIndex();

	//ī�޶� ���� ������Ʈ
	cameraUpdate();

	setTrigger();

	//cout << "x : " << (int)(_player->getPlayerLocX()) / TILESIZE << " y : " << (int)(_player->getPlayerLocY()) / TILESIZE << endl;
}

void garden_5f::render()
{	
	//��� ����
	IMAGEMANAGER->FindImage("���60")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("���60"),
		Vector2(IMAGEMANAGER->FindImage("���60")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("���60")->GetSize().y / 2));

	fifthFloorStage::render();


}

void garden_5f::Collision()
{
}

void garden_5f::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/5f_garden.map", GENERIC_READ, NULL, NULL,
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

void garden_5f::setTrigger()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// Ŭ���ൿ Ʈ����
	{
		if (IntersectRectToRect(&_tiles[FLOWER].rc, &_player->getSearchRc()))
		{
			cout << "��!" << endl;
		}
		if (IntersectRectToRect(&_tiles[NEKO].rc, &_player->getSearchRc()))
		{
			cout << "���̺�!" << endl;
		}
		if (IntersectRectToRect(&_tiles[TREE].rc, &_player->getSearchRc()) ||
			IntersectRectToRect(&_tiles[TREE + 1].rc, &_player->getSearchRc()))
		{
			cout << "����!" << endl;
		}
	}

	if (IntersectRectToRect(&_tiles[DOORTOGARDENTOBOSS].rc, &_player->getPlayerFrc()))
	{
		_vFrameTile[0].isTrigger = true;
		sceneChange("gardenToBoss_5f");
		cout << "������!" << endl;
	}
	if (IntersectRectToRect(&_tiles[DOORTOPRISON].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISON + TILEX].rc, &_player->getPlayerFrc()))
	{
		sceneChange("prison_5f");
		cout << "��������!" << endl;
	}
	if (IntersectRectToRect(&_tiles[DOORTODININGROOM].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTODININGROOM + TILEX].rc, &_player->getPlayerFrc()))
	{
		sceneChange("diningRoom_5f");
		cout << "�ȹ�����!" << endl;
	}
}

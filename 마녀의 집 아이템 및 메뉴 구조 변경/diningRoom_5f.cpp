#include "stdafx.h"
#include "diningRoom_5f.h"
#include "Player.h"

HRESULT diningRoom_5f::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_player->setDirec(CHRDIREC_LEFT);

	//Ÿ�� �ҷ�����
	load(_location);

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	fifthFloorStage::init();

	return S_OK;
}

void diningRoom_5f::release()
{
}

void diningRoom_5f::update()
{
	fifthFloorStage::update();

	setFrameIndex();

	//ī�޶� ���� ������Ʈ
	cameraUpdate();
	setTrigger();
	//cout << "x : " << (int)(_player->getPlayerLocX()) / TILESIZE << " y : " << (int)(_player->getPlayerLocY()) / TILESIZE << endl;
}

void diningRoom_5f::render()
{	
	//��� ����
	IMAGEMANAGER->FindImage("���62")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("���62"),
		Vector2(IMAGEMANAGER->FindImage("���62")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("���62")->GetSize().y / 2));

	fifthFloorStage::render();

}

void diningRoom_5f::Collision()
{
}

void diningRoom_5f::load(LOCATION _location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/5f_diningRoom.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute != PLAYER) continue;

		//�ʱ� ��ġ�� ����ش�.
		switch (_location)
		{
		case LOCATION_DEFAULT: default:
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}
		break;
	}
	CloseHandle(file);
}

void diningRoom_5f::setTrigger()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// Ŭ���ൿ Ʈ����
	{
		if (IntersectRectToRect(&_tiles[Y_FLOWER_1].rc, &_player->getSearchRc()))
		{
			cout << "��1!" << endl;
		}
		if (IntersectRectToRect(&_tiles[Y_FLOWER_2].rc, &_player->getSearchRc()))
		{
			cout << "��2!" << endl;
		}
		if (IntersectRectToRect(&_tiles[Y_FLOWER_3].rc, &_player->getSearchRc()))
		{
			cout << "��3!" << endl;
		}
		if (IntersectRectToRect(&_tiles[Y_FLOWER_4].rc, &_player->getSearchRc()))
		{
			cout << "��4!" << endl;
		}
		if (IntersectRectToRect(&_tiles[CLOCK].rc, &_player->getSearchRc()))
		{
			cout << "�ð�!" << endl;
		}
		if (IntersectRectToRect(&_tiles[LIGHT].rc, &_player->getSearchRc()))
		{
			cout << "����!" << endl;
		}
		if (IntersectRectToRect(&_tiles[DOORTODININGROOMITEM].rc, &_player->getPlayerFrc()))
		{
			cout << "������ȹ��!!" << endl;
		}
	}

	if (IntersectRectToRect(&_tiles[DOORTOGARDEN].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOGARDEN + TILEX].rc, &_player->getPlayerFrc()))
	{
		_isChangeScene = true;
		sceneChange("garden_5f", CHRDIREC_RIGHT, LOCATION_1);
		cout << "��������!" << endl;
	}
	if (IntersectRectToRect(&_tiles[DROPFLOWER].rc, &_player->getPlayerFrc()))
	{
		_vFrameTile[2].isTrigger = true;
		cout << "ȭ��!" << endl;
	}
}

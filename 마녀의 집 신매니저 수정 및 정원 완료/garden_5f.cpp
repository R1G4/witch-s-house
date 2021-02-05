#include "stdafx.h"
#include "garden_5f.h"
#include "player.h"

HRESULT garden_5f::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_player->setDirec(_chrdirection);

	//Ÿ�� �ҷ�����
	load(_location);

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

void garden_5f::load(LOCATION _location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/5f_garden.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute != PLAYER) continue;

		//�ʱ� ��ġ�� ����ش�.
		switch (_location)
		{
		case LOCATION_1:
			_player->setStart((DOORTODININGROOM + 1) % TILEX, (DOORTODININGROOM + 1) / TILEX);
			break;
		case LOCATION_2:
			_player->setStart((DOORTOGARDENTOBOSS + TILEX) % TILEX, (DOORTOGARDENTOBOSS + TILEX) / TILEX);
			break;		
		case LOCATION_3:
			_player->setStart((DOORTOPRISON - 1) % TILEX, (DOORTOPRISON - 1) / TILEX);
			break;
		case LOCATION_DEFAULT: default:
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}
		break;
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

	//cout << _vFrameTile[1].keyName << endl;
	if (IntersectRectToRect(&_tiles[DOORTOGARDENTOBOSS].rc, &_player->getPlayerFrc()))
	{
		_isChangeScene = true;
		_vFrameTile[1].isTrigger = true;
		sceneChange("gardenToBoss_5f", CHRDIREC_UP, LOCATION_DEFAULT);
		cout << "������!" << endl;
	}
	if (IntersectRectToRect(&_tiles[DOORTOPRISON].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISON + TILEX].rc, &_player->getPlayerFrc()))
	{
		_isChangeScene = true;
		sceneChange("prison_5f", CHRDIREC_RIGHT, LOCATION_DEFAULT);
		cout << "��������!" << endl;
	}
	if (IntersectRectToRect(&_tiles[DOORTODININGROOM].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTODININGROOM + TILEX].rc, &_player->getPlayerFrc()))
	{
		_isChangeScene = true;
		sceneChange("diningRoom_5f", CHRDIREC_LEFT, LOCATION_DEFAULT);
		cout << "�ȹ�����!" << endl;
	}
}

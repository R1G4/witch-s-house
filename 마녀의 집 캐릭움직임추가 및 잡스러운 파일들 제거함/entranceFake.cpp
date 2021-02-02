#include "stdafx.h"
#include "entranceFake.h"
#include "player.h"


entranceFake::entranceFake()
{
}


entranceFake::~entranceFake()
{
}

HRESULT entranceFake::init()
{
	_player->setDirec(CHRDIREC_UP);

	//Ÿ�� �ҷ�����
	load();

	firstFloorStage::init();

	return S_OK;
}

void entranceFake::release()
{
	firstFloorStage::release();
}

void entranceFake::update()
{
	//������ �ε��� ����
	setFrameIndex();

	switch (_trigger)
	{
	case entranceFake::DOOR_CLOSE: case entranceFake::NONE:
		//���� ���� ���¶�� �浹 �� �÷��̾� ���� �����ϰ� �Ѵ�.
		firstFloorStage::update();
		//�浹ó��
		Collision();
		break;
	case entranceFake::DOOR_OPEN:
		//���� ���� ���¶�� ȭ�� ���� �����Ͽ� ���� ������ ���� �� ��� �� ��ȯ�� �Ѵ�.
		firstFloorStage::sceneChange("entranceTrap");
		break;
	}

	//ī�޶� ���� ������Ʈ
	firstFloorStage::cameraUpdate();
}

void entranceFake::render()
{
	//��� ����
	IMAGEMANAGER->FindImage("���6")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("���6"),
		Vector2(IMAGEMANAGER->FindImage("���6")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("���6")->GetSize().y / 2));

	firstFloorStage::render();
}

void entranceFake::Collision()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			int index = i * TILEX + j;

			//��� Ÿ�ϰ� �浹���� ���� ��� ����
			if (!IntersectRectToRect(&_tiles[index].rc, &_player->getPlayerFrc())) continue;

			cout << "x: " << j << "  y: " << i << "  index: " << index << endl;

			//Ÿ�� �浹(�̵��� ���ϴ� Ÿ��)�� �����Ƿ� ������ Ŭ�������� ������.
			firstFloorStage::tileCollision(i, j);

			//�ش� Ÿ���� �ӿ� ����
			switch (_tiles[index].terrain)
			{
			case TR_TRIGGER:
				//���� Ÿ���� ������ �̹����� ��.. �ӽñ���
				if (_tiles[index].keyName.find("��") != string::npos && _tiles[index].keyName.size() <= 3)
				{
					//�ش� ������ �̹����� ã�Ƽ�
					for (int k = 0; k < _vFrameTile.size(); k++)
					{
						if (_vFrameTile[k].indexX == i && _vFrameTile[k].indexY == j && _vFrameTile[k].keyName == _tiles[index].keyName)
						{
							//Ʈ���Ÿ� �ߵ��Ѵ�.
							_vFrameTile[k].isTrigger = true;
							_trigger = DOOR_OPEN;
						}
					}
				}
				break;
			}
		}
	}
}

void entranceFake::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/entranceFake.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute == PLAYER)
		{
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}
	}
	CloseHandle(file);
}
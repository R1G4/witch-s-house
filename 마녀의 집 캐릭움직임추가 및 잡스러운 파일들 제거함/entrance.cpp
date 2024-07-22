#include "stdafx.h"
#include "entrance.h"
#include "Player.h"


entrance::entrance()
{
}


entrance::~entrance()
{
}

HRESULT entrance::init()
{
	_player->setDirec(CHRDIREC_DOWN);

	//Ÿ�� �ҷ�����
	load();

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	firstFloorStage::init();
	return S_OK;
}

void entrance::release()
{
	firstFloorStage::release();
}

void entrance::update()
{
	//�浹ó��
	Collision();
	//������ �ε��� ����
	setFrameIndex();

	firstFloorStage::cameraUpdate();
	firstFloorStage::update();
	//switch (_trigger)
	//{
	//case entrance::DOOR_CLOSE: case entrance::NONE:
	//	//���� ���� ���¶�� �浹 �� �÷��̾� ���� �����ϰ� �Ѵ�.
	//	firstFloorStage::update();
	//	Collision();
	//	break;
	//case entrance::DOOR_OPEN:
	//	//���� ���� ���¶�� ȭ�� ���� �����Ͽ� ���� ������ ���� �� ��� �� ��ȯ�� �Ѵ�.
	//	firstFloorStage::sceneChange("stage2_entranceTrap");
	//	break;
	//case entrance::READ:
	//	//�߰�����
	//	//���󼣶�~ ���⼭ ���̾�α� ���������
	//	//�д� �̼� ����
	//	_mission.read = SUCCESS;
	//	_trigger = NONE;
	//	break;
	//}
}

void entrance::render()
{
	//��� ����
	IMAGEMANAGER->FindImage("���8")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("���8"),
		Vector2(IMAGEMANAGER->FindImage("���8")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("���8")->GetSize().y / 2));

	firstFloorStage::render();
}

void entrance::Collision()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			int index = i * TILEX + j;
			//��� Ÿ�ϰ� �浹 ���� ���
			FloatRect pFrc = _player->getPlayerFrc();
			if (!IntersectRectToRect(&_tiles[index].rc, &pFrc)) continue;

			//Ÿ�� �浹(�̵��� ���ϴ� Ÿ��)�� �����Ƿ� ������ Ŭ�������� ������.
			firstFloorStage::tileCollision(i, j);

			//�ش� Ÿ���� �ӿ� ����
			//���Ŀ� Ÿ�� �Ӽ� ���ܰ� ���� ��� ����ġ������ if������ ���� �� ������
			switch (_tiles[index].terrain)
			{
			case TR_TRIGGER:
				cout << "x: " << i << "  y: " << j << "  index: " << index << endl;

				//Ʈ���Ÿ� �޾ƿ´�
				//if(KEYMANAGER->isOnceKeyDown(VK_SPACE))
				_trigger = (TRIGGER)index;

				//Ʈ���ſ� ������ �̹����� ���� ��ġ�� ���
				//�ش� ������ �̹����� ã�Ƽ�
				for (int k = 0; k < _vFrameTile.size(); k++)
				{
					if (_vFrameTile[k].indexX == i && _vFrameTile[k].indexY == j && _vFrameTile[k].keyName == _tiles[index].keyName)
					{
						//Ʈ���Ÿ� �ߵ��Ѵ�.
						_vFrameTile[k].isTrigger = true;
					}
				}
				break;
			}
		}
	}
}

void entrance::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/entrance.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute == PLAYER)
		{
			cout << i % TILEX << "&&, "<< i / TILEX << endl;
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}
	}
	CloseHandle(file);
}
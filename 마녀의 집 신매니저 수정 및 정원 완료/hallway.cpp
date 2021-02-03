#include "stdafx.h"
#include "hallway.h"
#include "player.h"

hallway::hallway()
{
}


hallway::~hallway()
{
}

HRESULT hallway::init()
{
	_player->setDirec(CHRDIREC_RIGHT);

	//Ÿ�� �ҷ�����
	load();

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	firstFloorStage::init();

	return S_OK;
}

void hallway::release()
{
	firstFloorStage::release();
}

void hallway::update()
{	
	//������ �ε��� ����
	setFrameIndex();;

	//switch (_trigger)
	//{
	//case hallway::DOOR_CLOSE: case hallway::NONE:
	//	//���� ���� ���¶�� �浹 �� �÷��̾� ���� �����ϰ� �Ѵ�.
	//	firstFloorStage::update();
	//	Collision();
	//	break;
	//case hallway::DOOR_OPEN:
	//	//���� ���� ���¶�� ȭ�� ���� �����Ͽ� ���� ������ ���� �� ��� �� ��ȯ�� �Ѵ�.
	//	firstFloorStage::sceneChange("entrance");
	//	break;
	//case hallway::READ:
	//	//�߰�����
	//	//���󼣶�~ ���⼭ ���̾�α� ���������
	//	//�д� �̼� ����
	//	_mission.read = SUCCESS;
	//	_trigger = NONE;
	//	break;
	//}

	//ī�޶� ���� ������Ʈ
	firstFloorStage::cameraUpdate();
}

void hallway::render()
{
	//��� ����
	IMAGEMANAGER->FindImage("���7")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("���7"),
		Vector2(IMAGEMANAGER->FindImage("���7")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("���7")->GetSize().y / 2));

	firstFloorStage::render();
}

void hallway::Collision()
{
	//for (int i = 0; i < TILEY; i++)
	//{
	//	for (int j = 0; j < TILEX; j++)
	//	{
	//		int index = i * TILEX + j;

	//		//��� Ÿ�ϰ� �浹 ���� ���
	//		if (!IntersectRectToRect(&_tiles[index].rc, &_player->getPlayerFrc())) continue;
	//		//Ÿ�� �浹(�̵��� ���ϴ� Ÿ��)�� �����Ƿ� ������ Ŭ�������� ������.
	//		firstFloorStage::tileCollision(i, j);
	//		//�ش� Ÿ���� �Ӽ��� ����
	//		//���Ŀ� Ÿ�� �Ӽ� ���ܰ� ���� ��� ����ġ������ if������ ���� �� ������
	//		switch (_tiles[index].terrain)
	//		{
	//		case TR_TRIGGER:
	//			//Ʈ���Ÿ� �޾ƿ´�
	//			_trigger = index == DOOR_OPEN ? (_mission.read ? DOOR_OPEN : _trigger) :
	//				index == READ ? (!_mission.read ? READ : _trigger) : _trigger;

	//			//Ʈ���ſ� ������ �̹����� ���� ��ġ�� ���
	//			//�ش� ������ �̹����� ã�Ƽ�
	//			for (int k = 0; k < _vFrameTile.size(); k++)
	//			{
	//				if (_vFrameTile[k].indexX == i && _vFrameTile[k].indexY == j && _vFrameTile[k].keyName == _tiles[index].keyName)
	//				{
	//					//Ʈ���Ÿ� �ߵ��Ѵ�.
	//					_vFrameTile[k].isTrigger = true;
	//				}
	//			}
	//			break;
	//		}

	//	}
	//}
}

void hallway::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/hallway.map", GENERIC_READ, NULL, NULL,
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
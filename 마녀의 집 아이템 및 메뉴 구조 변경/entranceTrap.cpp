#include "stdafx.h"
#include "entranceTrap.h"
#include "player.h"


entranceTrap::entranceTrap()
{
}


entranceTrap::~entranceTrap()
{
}

HRESULT entranceTrap::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_player->setDirec(_chrdirection);

	//Ÿ�� �ҷ�����
	load();

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	firstFloorStage::init();

	_trigger = NONE;

	return S_OK;
}

void entranceTrap::release()
{
	firstFloorStage::release();
}

void entranceTrap::update()
{	
	//������ �ε��� ����
	setFrameIndex();;

	switch (_trigger)
	{
	case entranceTrap::NONE:
		//���� ���� ���¶�� �浹 �� �÷��̾� ���� �����ϰ� �Ѵ�.
		firstFloorStage::update();
		firstFloorStage::setAlpha();
		Collision();
		break;
	case entranceTrap::DOOR_OPEN:
		//���� ���� ���¶�� ȭ�� ���� �����Ͽ� ���� ������ ���� �� ��� �� ��ȯ�� �Ѵ�.
		firstFloorStage::sceneChange("entrance", CHRDIREC_DOWN, LOCATION_DEFAULT);
		break;
	case entranceTrap::READ:
		firstFloorStage::setAlpha();
		//�߰�����
		//���󼣶�~ ���⼭ ���̾�α� ���������
		//�д� �̼� ����
		_mission.read = SUCCESS;
		_trigger = NONE;
		break;
	default:
		_trigger = NONE;
		firstFloorStage::setAlpha();
		firstFloorStage::update();
		Collision();
		break;
	}
	
	//ī�޶� ���� ������Ʈ
	firstFloorStage::cameraUpdate();
}

void entranceTrap::render()
{
	//��� ����
	IMAGEMANAGER->FindImage("���7")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("���7"),
		Vector2(IMAGEMANAGER->FindImage("���7")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("���7")->GetSize().y / 2));

	firstFloorStage::render();
}

void entranceTrap::Collision()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			int index = i * TILEX + j;

			//��� Ÿ�ϰ� �浹 ���� ���
			if (!IntersectRectToRect(&_tiles[index].rc, &_player->getPlayerFrc())) continue;
			//Ÿ�� �浹(�̵��� ���ϴ� Ÿ��)�� �����Ƿ� ������ Ŭ�������� ������.
			firstFloorStage::tileCollision(i, j);
			//�ش� Ÿ���� �Ӽ��� ����
			//���Ŀ� Ÿ�� �Ӽ� ���ܰ� ���� ��� ����ġ������ if������ ���� �� ������
			switch (_tiles[index].terrain)
			{
			case TR_TRIGGER:
				//Ʈ���Ÿ� �޾ƿ´�
				_trigger = index == DOOR_OPEN ? (_mission.read ? DOOR_OPEN : _trigger) :
					index == READ ? (!_mission.read ? READ : _trigger) : _trigger;

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

void entranceTrap::load(LOCATION location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/1f_entranceTrap.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute != PLAYER)
			continue;

		if (location)	_player->setStart(i%TILEX, i / TILEX);
		break;
	}
	CloseHandle(file);
}
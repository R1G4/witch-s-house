#include "stdafx.h"
#include "scissorsRoom.h"
#include "Player.h"

scissorsRoom::scissorsRoom()
{
}

scissorsRoom::~scissorsRoom()
{
}

HRESULT scissorsRoom::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_player->setDirec(_chrdirection);

	//Ÿ�� �ҷ�����
	load();

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	firstFloorStage::init();

	_trigger = NONE;

	return S_OK;
}

void scissorsRoom::release()
{
	firstFloorStage::release();
}

void scissorsRoom::update()
{
	//������ �ε��� ����
	setFrameIndex();

	switch (_trigger)
	{
	case scissorsRoom::NONE:
		firstFloorStage::update();
		Collision();
		break;
	case scissorsRoom::DOOR_RIGHT_OPEN:
		firstFloorStage::sceneChange("entrance", CHRDIREC_RIGHT, LOCATION_2);
		break;

	default:
		_trigger = NONE;
		firstFloorStage::update();
		Collision();
		break;
	}

	firstFloorStage::cameraUpdate();
}

void scissorsRoom::render()
{
	//��� ����
	IMAGEMANAGER->FindImage("���14")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("���14"),
		Vector2(IMAGEMANAGER->FindImage("���14")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("���14")->GetSize().y / 2));

	firstFloorStage::render();
}

void scissorsRoom::Collision()
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

			//�ش� Ÿ���� �ӿ� ����
			//���Ŀ� Ÿ�� �Ӽ� ���ܰ� ���� ��� ����ġ������ if������ ���� �� ������
			cout << "x: " << i << "  y: " << j << "  index: " << index << endl;

			switch (_tiles[index].terrain)
			{
			case TR_TRIGGER:
				cout << "x: " << i << "  y: " << j << "  index: " << index << endl;
				//Ʈ���� �޾ƿ���
				_trigger = index == 564 ? DOOR_RIGHT_OPEN : (TRIGGER)index;

				break;
			}
		}
	}
}

void scissorsRoom::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/scissorsRoom.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute == PLAYER)
		{
			cout << i % TILEX << "&&, " << i / TILEX << endl;
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}
	}
	CloseHandle(file);
}

void scissorsRoom::getMemory()
{
	for (int k = 0; k < _vFrameTile.size(); k++)
	{
		if (!STAGEMEMORYMANAGER->getIsCandle())
			continue;

		if (_vFrameTile[k].keyName == "����")
		{
			//Ʈ���Ű� �̹� �ߵ��Ǿ��� ���·� �����Ѵ�.
			_vFrameTile[k].isMaxframe = true;
		}
		else if (_vFrameTile[k].keyName == "��")
		{
			//���ʹ� Ʈ���� �ߵ�
		}
	}
}
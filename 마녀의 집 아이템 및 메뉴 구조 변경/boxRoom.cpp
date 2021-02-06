#include "stdafx.h"
#include "boxRoom.h"
#include "Player.h"


boxRoom::boxRoom()
{
}


boxRoom::~boxRoom()
{
}

HRESULT boxRoom::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_player->setDirec(_chrdirection);

	//Ÿ�� �ҷ�����
	load();

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	firstFloorStage::init();
	_trigger = NONE;

	return S_OK;
}

void boxRoom::release()
{
	firstFloorStage::release();
}

void boxRoom::update()
{
	//������ �ε��� ����
	setFrameIndex();
	
	switch (_trigger)
	{
	case boxRoom::NONE:
		firstFloorStage::update();
		firstFloorStage::setAlpha();
		Collision();
		break;
	case boxRoom::DOOR_LEFT_OPEN:
		firstFloorStage::sceneChange("hallway", CHRDIREC_LEFT, LOCATION_1);
		break;
	case boxRoom::BOX:
		firstFloorStage::setAlpha();
		STAGEMEMORYMANAGER->setIsBox(true);
		_trigger = DELAY;
		break;
	case boxRoom::DELAY:
		firstFloorStage::setAlpha();
		_player->setState(CHR_IDLE);
		_delay++;
		if (_delay % 70 == 0)
			_trigger = NONE;
		break;
	case boxRoom::BEAR_PICKUP:
		STAGEMEMORYMANAGER->setIsBearPickUp(true);
		_trigger = NONE;
		break;
	default:
		_trigger = NONE;
		firstFloorStage::update();
		firstFloorStage::setAlpha();
		Collision();
		break;
	}

	firstFloorStage::cameraUpdate();
}

void boxRoom::render()
{
	//��� ����
	IMAGEMANAGER->FindImage("���13")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("���13"),
		Vector2(IMAGEMANAGER->FindImage("���13")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("���13")->GetSize().y / 2));

	firstFloorStage::render();
}

void boxRoom::Collision()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			int index = i * TILEX + j;

			//��� Ÿ�ϰ� �÷��̾� ��ȣ�ۿ� ��Ʈ�� �浹�Ͽ��ٸ�
			if (IntersectRectToRect(&_tiles[index].rc, &_player->getSearchRc()))
			{
				//Ÿ���� �ε����� �ش� �������� ���� �ϰ� ȹ���� �õ��Ѵٸ�
				if (!STAGEMEMORYMANAGER->getIsBearPut() && !STAGEMEMORYMANAGER->getIsBearPickUp())

				if ((TRIGGER)index == BEAR_PICKUP)
				{
					//�ؽ��� �ִ� ���ÿ� �� ����
					if (SelectionForm(L"�������� �ݴ´�.", L"�ƹ��͵� ���� �ʴ´�.") && _fromSelected == LEFT)
					{
						//������ �Ŵ��� - ������ ȹ��
						//������ Ű(image.find(key))�� Ȥ�� ������ ���ϸ��� �ִ´�
						//���� �������� �̹� �����ϰų� �Ŵ��� �ȿ� Ű���� ���� ���� �ʴ� �� ������ ��� false�� ��ȯ�Ѵ�. true�� ��� �߰�(ȹ��)
						if (!ITEMMANAGER->addItem(_tiles[index].keyName)) continue;

						_trigger = BEAR_PICKUP;
					}

				}
			}
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
				if((TRIGGER)index != BOX && (TRIGGER)index != BEAR_PICKUP && index != 558 )
					_trigger = index == 556 ? DOOR_LEFT_OPEN : (TRIGGER)index;
				
				for (int k = 0; k < _vFrameTile.size(); k++)
				{
					if (_vFrameTile[k].isMaxframe || _vFrameTile[k].isTrigger)
						continue;

					if (((TRIGGER)index == BOX || index == 500) && _vFrameTile[k].keyName == "���ھ�����")
					{
						//Ʈ���Ÿ� �ߵ��Ѵ�.
						_vFrameTile[k].isTrigger = true;
						_trigger = BOX;
					}
				}
				break;
			}
		}
	}
}

void boxRoom::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/1f_boxRoom.map", GENERIC_READ, NULL, NULL,
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
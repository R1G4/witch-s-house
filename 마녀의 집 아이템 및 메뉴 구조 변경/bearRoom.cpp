#include "stdafx.h"
#include "bearRoom.h"
#include "Player.h"


bearRoom::bearRoom()
{
}


bearRoom::~bearRoom()
{
}

HRESULT bearRoom::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_player->setDirec(_chrdirection);

	//Ÿ�� �ҷ�����
	load();

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	firstFloorStage::init();
	_trigger = NONE;
	getMemory();
	setFrameIndex();
	return S_OK;
}

void bearRoom::release()
{
	firstFloorStage::release();
}

void bearRoom::update()
{
	//������ �ε��� ����
	setFrameIndex();

	switch (_trigger)
	{
	case bearRoom::NONE:
		firstFloorStage::update();
		firstFloorStage::setAlpha();
		Collision();
		break;
	case bearRoom::DOOR_LEFT_OPEN:
		firstFloorStage::sceneChange("hallway", CHRDIREC_LEFT, LOCATION_2);
		break;
	case bearRoom::BEAR_PUT:
		STAGEMEMORYMANAGER->setIsBearPut(true);
		_trigger = NONE;
		break;
	default:
		_trigger = NONE;
		firstFloorStage::setAlpha();
		firstFloorStage::update();
		Collision();
			break;
	}

	firstFloorStage::cameraUpdate();
}

void bearRoom::render()
{
	//��� ����
	IMAGEMANAGER->FindImage("���12")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("���12"),
		Vector2(IMAGEMANAGER->FindImage("���12")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("���12")->GetSize().y / 2));

	firstFloorStage::render();
}

void bearRoom::Collision()
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
				//�׽�Ʈ�� ������̹Ƿ� ���Ŀ� Ű�Ŵ����� ���� �����ؾ���
				if(!STAGEMEMORYMANAGER->getIsBearPut() && STAGEMEMORYMANAGER->getIsBearPickUp())
				if ((TRIGGER)index == BEAR_PUT)
				{
					for (int k = 0; k < _vFrameTile.size(); k++)
					{
						if (_vFrameTile[k].keyName == "�ٱ��ϰ�")
						{
							if (_vFrameTile[k].isMaxframe || _vFrameTile[k].isTrigger)	continue;

							//�ؽ��� �ִ� ���ÿ� �� ����
							if (SelectionForm(L"�������� �ִ´�", L"�ƹ��͵� ���� �ʴ´�.") && _fromSelected == LEFT)
							{
								//������ �Ŵ��� - ������ ���
								//������ Ű(image.find(key))�� Ȥ�� ������ ���ϸ��� �ִ´�
								//�÷��̾ ������ ����� �õ��Ѵٸ� ���� �������� ���ŵȴ�.
								//������ ����� �õ����� �ʰų� �ش� �������� ���� ���� �ʴٸ� false�� ��ȯ�Ѵ�. true�� ��� ����(���)
								if (!ITEMMANAGER->useItem(58)) continue;

								//����� �Ͽ����� Ʈ���Ÿ� �ߵ��Ѵ�.
								_vFrameTile[k].isTrigger = true;
								_trigger = BEAR_PUT;
							}
							break;
						}
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
				if(index != BEAR_PUT)
					_trigger = index == 556 ? DOOR_LEFT_OPEN : (TRIGGER)index;

				break;
			}
		}
	}
}

void bearRoom::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/1f_bearRoom.map", GENERIC_READ, NULL, NULL,
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

void bearRoom::getMemory()
{
	for (int k = 0; k < _vFrameTile.size(); k++)
	{
		if (_vFrameTile[k].keyName == "�ٱ��ϰ�" && STAGEMEMORYMANAGER->getIsBearPut())
		{
			//Ʈ���Ű� �̹� �ߵ��Ǿ��� ���·� �����Ѵ�.
			_vFrameTile[k].isMaxframe = true;
		}
	}
}
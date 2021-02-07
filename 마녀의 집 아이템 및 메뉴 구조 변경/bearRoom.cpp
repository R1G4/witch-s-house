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
	//�÷��̾ �ٶ󺸴� ����
	_player->setDirec(_chrdirection);

	//Ÿ�� �ҷ�����
	load();

	//ī�޶� ����
	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());

	//1�� ���� �������� �ʱ�ȭ
	firstFloorStage::init();

	//�ʱ� Ʈ���� ����
	_trigger = NONE;
	
	//�������� �޸� �ҷ�����
	getMemory();

	//������ �̹��� ������ ����
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

	//Ʈ���� ���¿� ���� ȣ�� �� ����
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
	case bearRoom::BEAR_ACCESS:
		_bearAccess = true;
		_player->setState(CHR_IDLE);
		int index;
		for (int i = 0; i < _vFrameTile.size(); i++)
		{
			if (_vFrameTile[i].keyName == "�ٱ��ϰ�")
			{
				index = i;
				break;
			}
		}
		_vFrameTile[index].indexX = (cnt % 3) + 1;
		_vFrameTile[index].rc.left -= 2;
		_vFrameTile[index].rc.right -= 2;
		cnt ++;

		if (cnt > 10)
		{
			_trigger = NONE;
			cnt = 0;
		}
		break;
	case bearRoom::READ:
		firstFloorStage::setAlpha();
		_vScript = TEXTMANAGER->loadFile("dialog/1f/1f_bearRoom.txt");

		if (KEYMANAGER->isOnceKeyUp(VK_SPACE) || KEYMANAGER->isOnceKeyUp('x'))
		{
			TEXTMANAGER->clearScript();
			_trigger = NONE;
			_vScript.clear();
		}
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
				if ((TRIGGER)index == READ && KEYMANAGER->isOnceKeyUp(VK_SPACE))
					_trigger = READ;

				//Ÿ���� �ε����� �ش� �������� ���� �ϰ� ȹ���� �õ��Ѵٸ�
				if(STAGEMEMORYMANAGER->getIsScissors() && !STAGEMEMORYMANAGER->getIsBearPut() && STAGEMEMORYMANAGER->getIsBearPickUp())
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
								if (!ITEMMANAGER->useItem(11)) continue;

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

			switch (_tiles[index].terrain)
			{
			case TR_TRIGGER:
				cout << "x: " << i << "  y: " << j << "  index: " << index << endl;

				if ((index == BEAR_ACCESS || index == 557) && !_bearAccess)
				{
					if (STAGEMEMORYMANAGER->getIsScissors() && STAGEMEMORYMANAGER->getIsBearPut() && STAGEMEMORYMANAGER->getIsBearPickUp())
						_trigger = BEAR_ACCESS;
				}

				//�׿� Ʈ���� �޾ƿ���
				if(index != BEAR_PUT && _trigger != READ && _trigger != BEAR_ACCESS)
					_trigger = index == 556 ? DOOR_LEFT_OPEN :
							   index == DOOR_LEFT_OPEN ? DOOR_LEFT_OPEN : NONE;

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
		if (_vFrameTile[k].keyName == "�չٴ�" && (STAGEMEMORYMANAGER->getIsPalmLeft() || (STAGEMEMORYMANAGER->getIsPalmRight())))
		{
			//Ʈ���Ű� �̹� �ߵ��Ǿ��� ���·� �����Ѵ�.
			_vFrameTile[k].isMaxframe = true;
		}
	}
}
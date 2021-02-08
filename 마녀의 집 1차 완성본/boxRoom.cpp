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

	//���� ������ �ʱ�ȭ�Ѵ�.
	_dead = new DeadManager;
	_dead->init();
	_dead->setPlayerAddress(_player);
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

	//Ʈ���� ���¿� ���� ȣ�� �� ����
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
	case boxRoom::READ:
		if (KEYMANAGER->isOnceKeyUp(VK_SPACE))
		{
			switch (_readCnt)
			{
			case boxRoom::FIRST:
				autoSound("openBook");
				_vScript = TEXTMANAGER->loadFile("dialog/1f/1f_boxRoom.txt");
				_readCnt = SECOND;
				break;
			case boxRoom::SECOND:
				autoSound("nextPage");
				TEXTMANAGER->setNextScript(true);
				_readCnt = THIRD;
				break;
			case boxRoom::THIRD:
				autoSound("nextPage");
				TEXTMANAGER->setNextScript(true);
				_readCnt = FOURTH;
				break;
			case boxRoom::FOURTH:
				autoSound("nextPage");
				TEXTMANAGER->setNextScript(true);
				_readCnt = END;
				break;
			case boxRoom::END:
				TEXTMANAGER->clearScript();
				_trigger = NONE;
				_vScript.clear();
				break;
			}
		}
		break;
	case boxRoom::THORN:
		firstFloorStage::update();
		firstFloorStage::setAlpha();
		_dead->setDead(DEAD_THORN);
		_dead->update();

		_delay++;
		//ǳ������ ���ؼ�.. ������ ����Ѵ�
		if (!(_delay % 57))
			autoSound("����");
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
				//�ؽ��� �ִ� ���ÿ� �� ����
				if (_readCnt != END && (TRIGGER)index == READ && SelectionForm(L"�д´�.", L"���� �ʴ´�") && _fromSelected == LEFT)
				{
					_trigger = READ;
				}
				//�ؽ��� �ִ� ���ÿ� �� ����
				if ((TRIGGER)index == THORN && SelectionForm(L"����� ������.", L"�׸��д�.") && _fromSelected == LEFT)
				{
					//���� �߰� ���� �����
					_trigger = THORN;
					autoSound("����");
				}
			}

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
							_tiles[index].obj = OBJ_NONE;
							_tiles[index].keyName = "";
							_trigger = BEAR_PICKUP;
							autoSound("getItem");
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
				//�׿� Ʈ���� �޾ƿ���
				_trigger = index == 556 ? DOOR_LEFT_OPEN :
					index == DOOR_LEFT_OPEN ? DOOR_LEFT_OPEN : NONE;

				if (_trigger == DOOR_LEFT_OPEN || _trigger == DOOR_LEFT_OPEN)
					autoSound("openDoarLong");
				for (int k = 0; k < _vFrameTile.size(); k++)
				{
					if (_vFrameTile[k].isMaxframe || _vFrameTile[k].isTrigger)
						continue;

					if (!STAGEMEMORYMANAGER->getIsBox() && ((TRIGGER)index == BOX || index == 500) && _vFrameTile[k].keyName == "���ھ�����")
					{
						//Ʈ���Ÿ� �ߵ��Ѵ�.
						_vFrameTile[k].isTrigger = true;
						_trigger = BOX;
						autoSound("�ڽ�");
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
			_player->setStart(i%TILEX, i / TILEX);
		}
	}
	CloseHandle(file);
}

void boxRoom::getMemory()
{
	if (STAGEMEMORYMANAGER->getIsBearPickUp())
	{
		_tiles[BEAR_PICKUP].obj = OBJ_NONE;
		_tiles[BEAR_PICKUP].keyName = "";

		for (int k = 0; k < _vFrameTile.size(); k++)
		{
			if (_vFrameTile[k].keyName == "���ھ�����")
			{
				//Ʈ���Ű� �̹� �ߵ��Ǿ��� ���·� �����Ѵ�.
				_vFrameTile[k].isMaxframe = true;
			}
		}
	}
}
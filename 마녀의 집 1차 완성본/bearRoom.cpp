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
			//������ �̵��Ѵ�.
			firstFloorStage::sceneChange("hallway", CHRDIREC_LEFT, LOCATION_2);
			break;
		case bearRoom::BEAR_PUT:
			//������ �ȴٸ� �߸� ���� ����ߴٸ� �޸𸮿��� �����Ѵ�.
			STAGEMEMORYMANAGER->setIsBearPut(true);

			//Ʈ���Ÿ� �ʱ�ȭ�Ѵ�.
			_trigger = NONE;
			break;	
		case bearRoom::BEAR_ACCESS:
			//��(�ٱ��� ��) ���� Ʈ���Ÿ� �����Ѵ�.
			_bearAccess = true;

			//�÷��̾��� ���¸� IDLE�� ��ȯ�Ѵ�.
			_player->setState(CHR_IDLE);

			//�ٱ��� ���� �����ϴ¸� �����ϴ� index�� ������ ��´�.
			int index;
			for (int i = 0; i < _vFrameTile.size(); i++)
			{
				if (_vFrameTile[i].keyName == "�ٱ��ϰ�")
				{
					index = i;
					break;
				}
			}

			//index�� ���� ������ �̿��Ͽ� ���� �̵���Ų��.
			_vFrameTile[index].indexX = (cnt % 3) + 1;
			_vFrameTile[index].rc.left -= 2;
			_vFrameTile[index].rc.right -= 2;

			//���� Ƚ���� ���� �̺�Ʈ�� �߻���Ų��.
			cnt ++;
			if (cnt == 2) autoSound("ȣ��2");
			if (cnt > 10)
			{
				_trigger = NONE;
				cnt = 0;
			}
			break;
		case bearRoom::READ:
			firstFloorStage::setAlpha();
			//���̾�α׸� �ҷ��´�.
			_vScript = TEXTMANAGER->loadFile("dialog/1f/1f_bearRoom.txt");

			//�ش� Ű�� ���� �� �ؽ�Ʈ�� �Ѿ�ų� �����Ѵ�.
			if (KEYMANAGER->isOnceKeyUp(VK_SPACE))
			{
				//Ʈ���Ÿ� �ʱ�ȭ�Ѵ�.
				_trigger = NONE;

				//���ʿ��� ���͸� ����.
				TEXTMANAGER->clearScript();
				_vScript.clear();
			}
			break;
		default:
			//�׿��� ���´� NONE�� ����.
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
			FloatRect pSRc = _player->getSearchRc();
			if (IntersectRectToRect(&_tiles[index].rc, &pSRc))
			{
				//�ش� Ÿ�� �Ӽ��� READ�̸鼭 �ش� Ű�� �Է��Ͽ��ٸ� Ʈ���� ���¸� READ�� ��ȯ�Ѵ�.
				if ((TRIGGER)index == READ && KEYMANAGER->isOnceKeyUp(VK_SPACE))
					_trigger = READ;

				//���� �ȴٸ��� �߶����� �ٱ��Ͽ� ���� ���� �ʾ����� ���� �ֿ��� ���� �ִٸ�
				if(STAGEMEMORYMANAGER->getIsScissors() && !STAGEMEMORYMANAGER->getIsBearPut() && STAGEMEMORYMANAGER->getIsBearPickUp())

				//�ش� Ÿ�� �Ӽ��� BEAR_PUT���
				if ((TRIGGER)index == BEAR_PUT)
				{
					for (int k = 0; k < _vFrameTile.size(); k++)
					{
						//Ÿ�� KEY�� �ٱ��ϰ��� �����Ѵٸ�
						if (_vFrameTile[k].keyName == "�ٱ��ϰ�")
						{
							//Ʈ���Ű� �̹� �߻��Ǿ��ų� Ȥ�� ������ ������ ����ǰ� �ִٸ� �Ʒ� �ڵ�� �����Ѵ�,.
							if (_vFrameTile[k].isMaxframe || _vFrameTile[k].isTrigger)	continue;

							//�ؽ��� �ִ� ���ÿ� �� ����
							if (SelectionForm(L"�������� �ִ´�", L"�ƹ��͵� ���� �ʴ´�.") && _fromSelected == LEFT)
							{
								//������ �Ŵ��� - ������ ���
								//������ Ű(image.find(key))�� Ȥ�� ������ ���ϸ��� �ִ´�
								//�÷��̾ ������ ����� �õ��Ѵٸ� ���� �������� ���ŵȴ�.
								//������ ����� �õ����� �ʰų� �ش� �������� ���� ���� �ʴٸ� false�� ��ȯ�Ѵ�. true�� ��� ����(���)
								if (!ITEMMANAGER->useItem(59)) continue;

								_vFrameTile[k].isTrigger = true;
								_trigger = BEAR_PUT;
							}
							break;
						}
					}
				}
			}

			//��� Ÿ�ϰ� �浹 ���� ���
			FloatRect pFrc = _player->getPlayerFrc();
			if (!IntersectRectToRect(&_tiles[index].rc, &pFrc)) continue;

			//Ÿ�� �浹(�̵��� ���ϴ� Ÿ��)�� �����Ƿ� ������ Ŭ�������� ������.
			firstFloorStage::tileCollision(i, j);

			switch (_tiles[index].terrain)
			{
			case TR_TRIGGER:

				if ((index == BEAR_ACCESS || index == 557) && !_bearAccess)
				{
					if (STAGEMEMORYMANAGER->getIsScissors() && STAGEMEMORYMANAGER->getIsBearPut() && STAGEMEMORYMANAGER->getIsBearPickUp())
						_trigger = BEAR_ACCESS;
				}

				//�׿� Ʈ���� �޾ƿ���
				if (_trigger != DOOR_LEFT_OPEN && index != BEAR_PUT && _trigger != READ && _trigger != BEAR_ACCESS)
				{
					_trigger = index == 556 ? DOOR_LEFT_OPEN :
						index == DOOR_LEFT_OPEN ? DOOR_LEFT_OPEN : NONE;

					if(_trigger == DOOR_LEFT_OPEN)
						autoSound("openDoarLong");
				}

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
			_player->setStart(i%TILEX, i / TILEX);
		}
	}
	CloseHandle(file);
}

void bearRoom::getMemory()
{	
	//�ʱ�ȭ �� ���ʹ� ��2 �� �̹� ���Դ� ���¶�� ���带 ����Ѵ�.
	if (STAGEMEMORYMANAGER->getIsBearComing2())
		autoSound("ȣ��3");

	for (int k = 0; k < _vFrameTile.size(); k++)
	{
		//Ÿ�Ͽ� �ٱ��ϰ��� �����ϸ� ������(��)�� ����� ���¶��
		if (_vFrameTile[k].keyName == "�ٱ��ϰ�" && STAGEMEMORYMANAGER->getIsBearPut())
		{
			//Ʈ���Ű� �̹� �ߵ��Ǿ��� ���·� �����Ѵ�.
			_vFrameTile[k].isMaxframe = true;
		}

		//Ÿ�Ͽ� �չٴ� �����ϸ� ���� �չٴ� Ʈ���Ű� �߻��Ǿ��ų� ������ �չٴ� Ʈ���Ű� �߻��Ǿ��ٸ�
		if (_vFrameTile[k].keyName == "�չٴ�" && (STAGEMEMORYMANAGER->getIsPalmLeft() || (STAGEMEMORYMANAGER->getIsPalmRight())))
		{
			//Ʈ���Ű� �̹� �ߵ��Ǿ��� ���·� �����Ѵ�.
			_vFrameTile[k].isMaxframe = true;
		}
	}
}
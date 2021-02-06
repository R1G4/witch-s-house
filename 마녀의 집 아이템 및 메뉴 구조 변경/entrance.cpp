#include "stdafx.h"
#include "entrance.h"
#include "Player.h"


entrance::entrance()
{
}


entrance::~entrance()
{
}

HRESULT entrance::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	//ĳ������ ��ġ��, �ٶ󺸴� ����
	_player->setDirec(_chrdirection);

	_bear = new bear;

	//Ÿ�� �ҷ�����
	load(_location);

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	firstFloorStage::init();

	_trigger = NONE;
	getMemory();
	setFrameIndex();

	return S_OK;
}

void entrance::release()
{
	firstFloorStage::release();
}

void entrance::getMemory()
{
	for (int k = 0; k < _vFrameTile.size(); k++)
	{
		if (_vFrameTile[k].keyName == "�����ʲ�����" && STAGEMEMORYMANAGER->getIsCandle())
		{
			//Ʈ���Ű� �̹� �ߵ��Ǿ��� ���·� �����Ѵ�.
			_vFrameTile[k].isMaxframe = true;
		}
		else if (_vFrameTile[k].keyName == "�ɺ�������" && STAGEMEMORYMANAGER->getIsVase())
		{
			//Ʈ���Ű� �̹� �ߵ��Ǿ��� ���·� �����Ѵ�.
			_vFrameTile[k].isMaxframe = true;
		}
		else if (_vFrameTile[k].keyName == "�չٴ�" && (STAGEMEMORYMANAGER->getIsPalmLeft() || (STAGEMEMORYMANAGER->getIsPalmRight())))
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

void entrance::update()
{
	//������ �ε��� ����
	setFrameIndex();
	switch (_trigger)
	{
	case entrance::NONE:
		_delay = 0;
		firstFloorStage::update();
		firstFloorStage::setAlpha();
		Collision();
		break;
	case entrance::DOOR_LEFT_OPEN:
		firstFloorStage::sceneChange("scissorsRoom", CHRDIREC_LEFT, LOCATION_DEFAULT);
		break;
	case entrance::DOOR_RIGHT_OPEN:	
		firstFloorStage::sceneChange("hallway", CHRDIREC_RIGHT, LOCATION_DEFAULT);
		break;
	case entrance::CAT_TALK:
		firstFloorStage::setAlpha();
		break;
	case entrance::CANDLE_OFF:
		firstFloorStage::setAlpha();
		STAGEMEMORYMANAGER->setIsCandle(true);
		_trigger = DELAY;
		break;
	case entrance::VASE_DOWN:
		firstFloorStage::setAlpha();
		STAGEMEMORYMANAGER->setIsVase(true);
		_trigger = DELAY;
		break;
	case entrance::PALM_LEFT:
		firstFloorStage::setAlpha();
		_isBlood = true;
		_trigger = DELAY;
		break;
	case entrance::PALM_RIGHT:
		firstFloorStage::setAlpha();
		_isBlood = true;
		_trigger = DELAY;
		break;
	case entrance::DELAY:
		firstFloorStage::setAlpha();
		_player->setState(CHR_IDLE);
		_delay++;
		if (_delay % 60 == 0)
			_trigger = NONE;
		break;
	default:
		_trigger = NONE;
		firstFloorStage::update();
		Collision();
		break;
	}	
	
	//ī�޶� ���� ������Ʈ
	firstFloorStage::cameraUpdate();
}

void entrance::render()
{
	//��� ����
	IMAGEMANAGER->FindImage("���8")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("���8"),
		Vector2(IMAGEMANAGER->FindImage("���8")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("���8")->GetSize().y / 2));

	firstFloorStage::render();

	_bear->render();
}

void entrance::Collision()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			int index = i * TILEX + j;
			//��� Ÿ�ϰ� �浹 ���� ���
			if (!IntersectRectToRect(&_tiles[index].rc, &_player->getPlayerFrc())) continue;
			cout << "x: " << j << "  y: " << i << "  index: " << index << endl;
			//Ÿ�� �浹(�̵��� ���ϴ� Ÿ��)�� �����Ƿ� ������ Ŭ�������� ������.
			firstFloorStage::tileCollision(i, j);
			//�ش� Ÿ���� �ӿ� ����
			//���Ŀ� Ÿ�� �Ӽ� ���ܰ� ���� ��� ����ġ������ if������ ���� �� ������
			switch (_tiles[index].terrain)
			{
			case TR_TRIGGER:
				
				if ((TRIGGER)index == PALM_LEFT || (TRIGGER)index == PALM_RIGHT)
				{
					//�����̸� �ٱ��Ͽ� ���� ���¶�� (�̱��� ���� �������� �ӽ������� true�� ����)
					if (_isBlood && STAGEMEMORYMANAGER->getIsBearPut())
					{
						for (int k = 0; k < _vFrameTile.size(); k++)
						{
							if (_vFrameTile[k].isMaxframe || _vFrameTile[k].isTrigger)
								continue;

							if ((TRIGGER)index == PALM_LEFT && _vFrameTile[k].keyName == "�չٴ�"
								&& ((_vFrameTile[k].indexX == 5 && _vFrameTile[k].indexY == 18) || (_vFrameTile[k].indexX == 7 && _vFrameTile[k].indexY == 17)))
							{
								//Ʈ���Ÿ� �ߵ��Ѵ�.
								_vFrameTile[k].isTrigger = true;
								_trigger = (TRIGGER)index;
								STAGEMEMORYMANAGER->setIsPalmLeft(true);
							}
							else if ((TRIGGER)index == PALM_RIGHT && _vFrameTile[k].keyName == "�չٴ�" && _vFrameTile[k].indexX == 7 && _vFrameTile[k].indexY == 27)
							{
								//Ʈ���Ÿ� �ߵ��Ѵ�.
								_vFrameTile[k].isTrigger = true;
								_trigger = (TRIGGER)index;
								//STAGEMEMORYMANAGER->setIsPalmRight(true);
								STAGEMEMORYMANAGER->setIsPalmRight(true);
							}
						}
					}//������ ������ ���¶�� (�̱��� �����̹Ƿ� true�� �ص�)
					else if(!_isBlood/*STAGEMEMORYMANAGER->getIsBearPickUp()*/)
						_trigger = (TRIGGER)index;
				}
				else if ((TRIGGER)index != CANDLE_OFF && (TRIGGER)index != VASE_DOWN)
				{
					_trigger = index == 556 ? DOOR_LEFT_OPEN : 
							   index == 568 ? DOOR_RIGHT_OPEN : (TRIGGER)index;

					break;
				}
				else
				{
					for (int k = 0; k < _vFrameTile.size(); k++)
					{
						if (_vFrameTile[k].isMaxframe || _vFrameTile[k].isTrigger)
							continue;

						if ((TRIGGER)index == CANDLE_OFF && _vFrameTile[k].keyName == "�����ʲ�����")
						{
							//Ʈ���Ÿ� �ߵ��Ѵ�.
							_vFrameTile[k].isTrigger = true;
							_trigger = CANDLE_OFF;
						}
						else if ((TRIGGER)index == VASE_DOWN && _vFrameTile[k].keyName == "�ɺ�������")
						{
							//Ʈ���Ÿ� �ߵ��Ѵ�.
							_vFrameTile[k].isTrigger = true;
							_trigger = VASE_DOWN;
						}
					}
				}
				break;
			}
		}
	}
}

void entrance::load(LOCATION location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/1f_entrance.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute != PLAYER)
			continue;

		//�ʱ� ��ġ�� ����ش�.
		switch (location)
		{
		case LOCATION_1:
			_player->setStart(507 % TILEX, 507 / TILEX);
			break;
		case LOCATION_2:
			_player->setStart(498 % TILEX, 498 / TILEX);
			break;
		case LOCATION_DEFAULT: default:
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}
		break;
	}
	CloseHandle(file);
}
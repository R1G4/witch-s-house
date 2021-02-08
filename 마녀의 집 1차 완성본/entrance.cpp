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
	//�÷��̾ �ٶ󺸴� ����
	_player->setDirec(_chrdirection);

	//Ÿ�� �ҷ�����
	load(_location);

	//ī�޶� ����
	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());

	//1�� ���� �������� �ʱ�ȭ
	firstFloorStage::init();

	//�ʱ� Ʈ���� ����
	_trigger = NONE;

	//���ʹ� ���� ���͹�
	_enemyInterval = 0;

	//�������� �޸� �ҷ�����
	getMemory();

	//������ �̹��� ������ ����
	setFrameIndex();

	return S_OK;
}

void entrance::bearCom()
{
	if (!STAGEMEMORYMANAGER->getIsBearComing() ||
		STAGEMEMORYMANAGER->getIsBearComing2() ||
		!STAGEMEMORYMANAGER->getIsBearPickUp() ||
		!STAGEMEMORYMANAGER->getIsBearPut())
		return;

	_enemyInterval++;
	if (_enemyInterval == 80)
	{
		_bear = new bear;
		_bear->init(496 % TILEX, 496 / TILEX);
		_playerTile = new astarTile;
		_enemyTile = new astarTile;
		_dead = new DeadManager;
		_dead->init();
		_dead->setPlayerAddress(_player);
		for (int k = 0; k < TILEX*TILEY; k++)
			_objTile[k] = new astarTile;
		bossLocX = 496 % TILEX;
		bossLocY = 496 / TILEX;

		firstFloorStage::objectLocation();

		STAGEMEMORYMANAGER->setIsBearComing2(true);
	}
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
	}
}

void entrance::update()
{
	bearCom();

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
		if(_bear) _player->setState(CHR_IDLE);
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
	if (_bear)	firstFloorStage::enemyUpdate();

	if(!_bear && _dead) _dead->update();
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
}

void entrance::Collision()
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
				if ((TRIGGER)index == CAT_TALK)
				{
					STORAGEMANAGER->loadView();

					//_trigger = CAT_TALK;
				}
			}


			//��� Ÿ�ϰ� �浹 ���� ���
			if (!IntersectRectToRect(&_tiles[index].rc, &_player->getPlayerFrc())) continue;


			firstFloorStage::tileCollision(i, j);
			//�ش� Ÿ���� �ӿ� ����
			//���Ŀ� Ÿ�� �Ӽ� ���ܰ� ���� ��� ����ġ������ if������ ���� �� ������
			switch (_tiles[index].terrain)
			{
			case TR_TRIGGER:
				
				if ((TRIGGER)index == PALM_LEFT || (TRIGGER)index == PALM_RIGHT)
				{
					if (_isBlood && STAGEMEMORYMANAGER->getIsScissors())
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
				else if ((TRIGGER)index != CANDLE_OFF && (TRIGGER)index != VASE_DOWN && (TRIGGER)index != CAT_TALK)
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
						//�����̸� �ٱ��Ͽ� ���� ����
						else if (STAGEMEMORYMANAGER->getIsBearPut() && (TRIGGER)index == VASE_DOWN && _vFrameTile[k].keyName == "�ɺ�������")
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
		switch (_tiles[i].attribute)
		{
			case PLAYER:
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
	}
	CloseHandle(file);
}
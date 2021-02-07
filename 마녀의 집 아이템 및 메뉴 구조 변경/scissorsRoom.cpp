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

	int temp = (int)"sdas";
	//Ÿ�� �ҷ�����
	load();

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	firstFloorStage::init();

	_trigger = NONE;

	getMemory();
	return S_OK;
}

void scissorsRoom::release()
{
	firstFloorStage::release();
}

void scissorsRoom::update()
{
	//
	//������ �ε��� ����
	setFrameIndex();
	switch (_trigger)
	{
	case scissorsRoom::NONE:
		_delay = 0;
		firstFloorStage::update();
		firstFloorStage::setAlpha();
		Collision();
		break;
	case scissorsRoom::DOOR_RIGHT_OPEN:
		firstFloorStage::sceneChange("entrance", CHRDIREC_RIGHT, LOCATION_2);
		break;
	case scissorsRoom::PALM:
		firstFloorStage::setAlpha();
		_isBlood = true;
		break;
	case scissorsRoom::SCISSORS:
 		STAGEMEMORYMANAGER->setIsScissors(true);
		//�ش� ������ �̹����� ã�Ƽ�
		for (int k = 0; k < _vFrameTile.size(); k++)
		{
			if (_vFrameTile[k].keyName.find("��") != string::npos && _vFrameTile[k].keyName.size() <= 3)
			{
				//Ʈ���Ÿ� �ߵ��Ѵ�.
				_vFrameTile[k].isTrigger = true;
				_vScript = TEXTMANAGER->loadFile("dialog/1f/1f_scissorsRoom.txt");
			}
		}
		_trigger = DELAY;
		break;	
	case scissorsRoom::BEARCOM:
		STAGEMEMORYMANAGER->setIsBearComing(true); 
		//STAGEMEMORYMANAGER->setIsBearComing2(true);
		_trigger = NONE;
		break;
	case scissorsRoom::DELAY:
		firstFloorStage::setAlpha();
		_player->setState(CHR_IDLE);
		_delay++;
		if (_delay % 60 == 0)
		{
			TEXTMANAGER->clearScript();
			if (_vScript.size() > 0) _vScript.clear();
			_trigger = NONE;
		}
		break;
	case scissorsRoom::DOOR_UP_OPEN:
		firstFloorStage::setAlpha();
		_isBlood = true;
		break;
	case scissorsRoom::DOOR_UP_OPEN2:
		SCENEMANAGER->changeScene("stairs_2F");
		//firstFloorStage::sceneChange("stairs_2F");
		break;
	default:
		_delay= 0 ;
		_trigger = NONE;
		firstFloorStage::update();
		firstFloorStage::setAlpha();
		Collision();
		break;
	}
	if(_bear)	firstFloorStage::enemyUpdate();
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

			//��� Ÿ�ϰ� �÷��̾� ��ȣ�ۿ� ��Ʈ�� �浹�Ͽ��ٸ�
			if (IntersectRectToRect(&_tiles[index].rc, &_player->getSearchRc()))
			{
				//���� ������ �����̸� ������ �ڸ��� �ʾҰ� �ٱ��Ͽ� ������ �ʾ������� 
				if (!STAGEMEMORYMANAGER->getIsScissors() && 
					!STAGEMEMORYMANAGER->getIsBearPut() &&
					!STAGEMEMORYMANAGER->getIsBearComing() &&
					STAGEMEMORYMANAGER->getIsBearPickUp())

				//�ؽ��� �ִ� ���ÿ� �� ����
				if ((TRIGGER)index == SCISSORS && SelectionForm(L"�������� �� �ٸ��� �ڸ���.", L"�ƹ��͵� ���� �ʴ´�.") && _fromSelected == LEFT)
				{
					//������ �Ŵ��� - ������ ���� �� �߰�
					//������ �������� key�� �ִ´�
					//���� ���� �� ���
					if (ITEMMANAGER->removeItem("obj58"))
					{
						//���� ������ �߰��Ѵ�.
						//�߰��� ���� �� ��� Ʈ���Ÿ� �������ش�.
						if (!ITEMMANAGER->addItem("obj11")) continue;

 						for (int k = 0; k < _vFrameTile.size(); k++)
						{
							if (_vFrameTile[k].isMaxframe || _vFrameTile[k].isTrigger)
								continue;

							if (_vFrameTile[k].keyName == "����")
							{
								//Ʈ���Ÿ� �ߵ��Ѵ�.
								_vFrameTile[k].isTrigger = true;
								_trigger = SCISSORS;
								break;
							}
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

				_trigger = index == 564 ? DOOR_RIGHT_OPEN :
					index == DOOR_RIGHT_OPEN ? DOOR_RIGHT_OPEN : NONE;

				for (int k = 0; k < _vFrameTile.size(); k++)
				{
					if (_vFrameTile[k].isMaxframe || _vFrameTile[k].isTrigger)
						continue;

					if (_isBlood && (TRIGGER)index == PALM && _vFrameTile[k].keyName == "�չٴ�")
					{
						//Ʈ���Ÿ� �ߵ��Ѵ�.
						_vFrameTile[k].isTrigger = true;
						_trigger = (TRIGGER)index;
						STAGEMEMORYMANAGER->setIsPalmRight(true);
					}
					//���� Ÿ���� ������ �̹����� ��.. �ӽñ���
					if (_tiles[index].keyName.find("��") != string::npos && _tiles[index].keyName.size() <= 3)
					{
						//�ش� ������ �̹����� ã�Ƽ�
						for (int k = 0; k < _vFrameTile.size(); k++)
						{
							if (_vFrameTile[k].indexX == i && _vFrameTile[k].indexY == j && _vFrameTile[k].keyName == _tiles[index].keyName)
							{
								if (STAGEMEMORYMANAGER->getIsBearComing() && STAGEMEMORYMANAGER->getIsBearComing2())
									_trigger = DOOR_UP_OPEN2;
								else
									_trigger = DOOR_UP_OPEN;
								//Ʈ���Ÿ� �ߵ��Ѵ�.
								_vFrameTile[k].isTrigger = true;
							}
						}
					}
				}
				break;
			}

			//������ �ȴٸ��� �ڸ� ���¶��
			if (STAGEMEMORYMANAGER->getIsScissors() &&
				!STAGEMEMORYMANAGER->getIsBearComing()&&
				!STAGEMEMORYMANAGER->getIsBearComing2()&&
				(TRIGGER)index == BEARCOM || (TRIGGER)index == 560)
			{
				_bear = new bear;
				_bear->init(319% TILEX, 319 / TILEX);
				_playerTile = new astarTile;
				_enemyTile = new astarTile;
				_dead = new DeadManager;
				_dead->init();
				_dead->setPlayerAddress(_player);
				for (int y = 0; y < TILEX*TILEY; y++)
					_objTile[y] = new astarTile;
				bossLocX = 319 % TILEX;
				bossLocY = 319 / TILEX;

				firstFloorStage::objectLocation();

				_trigger = BEARCOM;

				break;
			}
			
		}
	}
}

void scissorsRoom::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/1f_scissorsRoom.map", GENERIC_READ, NULL, NULL,
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

void scissorsRoom::getMemory()
{
	for (int k = 0; k < _vFrameTile.size(); k++)
	{
		if (STAGEMEMORYMANAGER->getIsPalmLeft() || STAGEMEMORYMANAGER->getIsPalmRight()) _isBlood = true;

		if (_vFrameTile[k].keyName == "����" && STAGEMEMORYMANAGER->getIsScissors())
		{
			//Ʈ���Ű� �̹� �ߵ��Ǿ��� ���·� �����Ѵ�.
			_vFrameTile[k].isMaxframe = true;
		}
		if (_vFrameTile[k].keyName == "�չٴ�" && STAGEMEMORYMANAGER->getIsPalm())
		{
			//Ʈ���Ű� �̹� �ߵ��Ǿ��� ���·� �����Ѵ�.
			_vFrameTile[k].isMaxframe = true;
		}
	}
}
#include "stdafx.h"
#include "prison_5f.h"
#include "Player.h"

HRESULT prison_5f::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_player->setDirec(_chrdirection);

	//Ÿ�� �ҷ�����
	load(_location); 

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	fifthFloorStage::init();

	getMemory();
	cout << STAGEMEMORYMANAGER->getIsPotion() << endl;
	_skulX = _tiles[SUMMONSKUL].rc.left / TILESIZE;
	_skulY = _tiles[SUMMONSKUL].rc.top / TILESIZE;
	_playerTile = new astarTile;
	_enemyTile = new astarTile;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		_objTile[i] = new astarTile;
	}
	objectLocation();
	return S_OK;
}

void prison_5f::release()
{
}

void prison_5f::update()
{
	if (!_isStopToRead)
	{
		fifthFloorStage::update();
		setFrameIndex();

		//ī�޶� ���� ������Ʈ
		cameraUpdate();
		setTrigger();
		
		if (_isSummon)
		{
			playerLocation();
			// ��Ÿ�� (ó���� ������ �����Ⱚ�̹Ƿ� ���⼭ �ѹ��� �����Ŵ)
			if (!_setTile)
			{
				enemyLocation();
				setAstarTile();
				_setTile = true;
			}

			_follow_count++;
			if (_follow_count >= 8)
			{
				resetEverything();
				_currentTile = _playerTile;
				while (_numCount == 0 && !_stop) pathFinder(_currentTile);
				if (Math::GetDistance(_playerTile->getCenter().x, _playerTile->getCenter().y, _enemyTile->getCenter().x, _enemyTile->getCenter().y) <= 48.0f)
				{
					_dead->setDead(DEAD_SKULL);
					_isDead = true;
					_skul->~skul();
					_isSummon = false;
					_stop = true;
					return;
				}
				_follow_count = 0;
			}
		}
	}
	else
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// Ŭ���ൿ Ʈ����
		{
			_isStopToRead = TEXTMANAGER->setNextScript(true);
		}
	}

	//cout << "x : " << (int)(_player->getPlayerLocX()) / TILESIZE << " y : " << (int)(_player->getPlayerLocY()) / TILESIZE << endl;
}

void prison_5f::render()
{	
	//��� ����
	IMAGEMANAGER->FindImage("���65")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("���65"),
		Vector2(IMAGEMANAGER->FindImage("���65")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("���65")->GetSize().y / 2));

	fifthFloorStage::render();

	if (_isStopToRead)
		TEXTMANAGER->renderText();
}

void prison_5f::Collision()
{
}

void prison_5f::load(LOCATION _location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/5f_prison.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute != PLAYER) continue;

		//�ʱ� ��ġ�� ����ش�.
		switch (_location)
		{
		case LOCATION_1:
			_player->setStart((DOORTOPRISONSKUL - 1) % TILEX, (DOORTOPRISONSKUL - 1) / TILEX);
			break;
		case LOCATION_2:
			_player->setStart((DOORTOPRISONWELL + TILEX) % TILEX, (DOORTOPRISONWELL + TILEX) / TILEX);
			break;
		case LOCATION_DEFAULT: default:
			_player->setStart(i % TILEX, i / TILEX);
			break;
		}
	}
	CloseHandle(file);
}

void prison_5f::setTrigger()
{
	if (_vFrameTile[4].isTrigger && _vFrameTile[4].frameY == 3 && !_isDrop)
	{
		_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_cage_1.txt");
		STAGEMEMORYMANAGER->setIsPotion(true);
		_isStopToRead = true;
		_isDrop = true;
	}

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// Ŭ���ൿ Ʈ����
	{
		_isStopToRead = TEXTMANAGER->setNextScript(true);

		if (IntersectRectToRect(&_tiles[R_FLOWER_1].rc, &_player->getSearchRc()))
		{
			STAGEMEMORYMANAGER->setIsRedFlower1(true);
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_flower_1.txt");
			_isStopToRead = true;
		}
		if (IntersectRectToRect(&_tiles[R_FLOWER_2].rc, &_player->getSearchRc()))
		{
			STAGEMEMORYMANAGER->setIsRedFlower2(true);
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_flower_2.txt");
			_isStopToRead = true;
		}
		if (IntersectRectToRect(&_tiles[R_FLOWER_3].rc, &_player->getSearchRc()))
		{
			STAGEMEMORYMANAGER->setIsRedFlower3(true);
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_flower_3.txt");
			_isStopToRead = true;
		}
		if (IntersectRectToRect(&_tiles[BOOK].rc, &_player->getSearchRc()))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_book.txt");
			_isStopToRead = true;
		}
		if (IntersectRectToRect(&_tiles[PAPER].rc, &_player->getSearchRc()))
		{
			cout << "��Ʈ!" << endl;
		}
		if (IntersectRectToRect(&_tiles[DOOR_2].rc, &_player->getSearchRc()))
		{
			switch (_ido)
			{
			case prison_5f::FIRST:
				_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_irondoor_1.txt");
				_isStopToRead = true;
				_ido = SECOND;
				break;
			case prison_5f::SECOND:
				STAGEMEMORYMANAGER->setIsGetSkul2(true);
				_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_irondoor_2.txt");
				_isStopToRead = true;
				_ido = THIRD;
			case prison_5f::THIRD:
				_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_irondoor_1.txt");
				_isStopToRead = true;
				break;
			default:
				break;
			}
			
		}
		if (IntersectRectToRect(&_tiles[CAGE].rc, &_player->getSearchRc()) && STAGEMEMORYMANAGER->getIsKey() == true)
		{
			_vFrameTile[4].isTrigger = true;
		}
		else if (IntersectRectToRect(&_tiles[CAGE].rc, &_player->getSearchRc()))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_cage.txt");
			_isStopToRead = true;
		}
	}

	if (IntersectRectToRect(&_tiles[SKUL].rc, &_player->getPlayerFrc()) && 
		!_isSummon && !_setTile && STAGEMEMORYMANAGER->getIsLever())
	{
		_skul->init(_skulX, _skulY);
		_isSkulAppeal = true;
		_isSummon = true;
	}
	if (IntersectRectToRect(&_tiles[DOORTOGARDEN].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOGARDEN + TILEX].rc, &_player->getPlayerFrc()))
	{
		_isChangeScene = true;
		sceneChange("garden_5f", CHRDIREC_LEFT, LOCATION_3);
	}
	if (IntersectRectToRect(&_tiles[DOORTOPRISONWELL].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISONWELL + 1].rc, &_player->getPlayerFrc()))
	{
		_vFrameTile[0].isTrigger = true;
		_isChangeScene = true;
		sceneChange("prison_well_5f", CHRDIREC_UP, LOCATION_DEFAULT);
	}
	if (IntersectRectToRect(&_tiles[DOORTOPRISONSKUL].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISONSKUL + TILEX].rc, &_player->getPlayerFrc()))
	{
		_isChangeScene = true;
		sceneChange("prison_skul_5f", CHRDIREC_RIGHT, LOCATION_DEFAULT);
	}
}

void prison_5f::getMemory()
{
	for (int k = 0; k < _vFrameTile.size(); k++)
	{
		if (!STAGEMEMORYMANAGER->getIsPotion()) continue;

		if (_vFrameTile[k].keyName == "����")
		{
			//Ʈ���Ű� �̹� �ߵ��Ǿ��� ���·� �����Ѵ�.
			_vFrameTile[k].isMaxframe = true;
		}
	}
}

void prison_5f::playerLocation()
{
	_playerTile->init(_player->getPlayerLocX() / TILESIZE,
		_player->getPlayerLocY() / TILESIZE);
	_playerTile->setAttribute("player");	// astarŸ�Ϸ� �÷��̾� ����
}

void prison_5f::enemyLocation()
{
	_enemyTile->init(_skulX, _skulY);			// astarŸ�Ϸ� �������� ����
	_enemyTile->setAttribute("enemy");			// astarŸ�Ϸ� ���ʹ� ����
}

void prison_5f::objectLocation()
{
	for (int i = 0; i < (TILEX * TILEY); i++)
	{
		if (_tiles[i].isCollider == true)
		{
			_objTile[MaxIndex]->init(_tiles[i].rc.left / TILESIZE, _tiles[i].rc.top / TILESIZE);
			MaxIndex++;
		}
	}
	cout << MaxIndex << endl;
	for (int i = 0; i < MaxIndex; i++)
	{
		_objTile[i]->setAttribute("wall");
	}
}

void prison_5f::setAstarTile()
{
	//���� Ÿ���� ����Ÿ�Ϸ�ٰ�
	_currentTile = _playerTile;

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			if (j == _playerTile->getIdx() && i == _playerTile->getIdy())
			{
				_playerTile->setColor(D2D1::ColorF::Green);		// �÷��̾� Ÿ�� ���� - ��ǻ� ���п� �ǹ̾�����
				_playerTile->setIsOpen(true);					// ���¸���Ʈ�ΰ�?
				_vTotalList.push_back(_playerTile);				// ��Ż����Ʈ�� �ֱ�
				continue;
			}

			if (j == _enemyTile->getIdx() && i == _enemyTile->getIdy())
			{
				_enemyTile->setColor(D2D1::ColorF::Red);		// ���ʹ� Ÿ�� ���� - ��ǻ� ���п� �ǹ̾�����
				_enemyTile->setIsOpen(true);					// ���¸���Ʈ�ΰ�?
				_vTotalList.push_back(_enemyTile);				// ��Ż����Ʈ�� �ֱ�
				continue;
			}

			//�� �� Ÿ��
			astarTile* node = new astarTile;
			node->init(j, i);
			node->setColor(D2D1::ColorF::White);
			node->setIsOpen(true);
			node->setAttribute("");

			// �� �� Ÿ�Ͽ��� ��ֹ��̸� ���� ���� �� ��� �Ұ����ϰ� ���� - ī�޶� ����Ǹ� �ٽ� ����
			for (int k = 0; k < MaxIndex; k++)
			{
				if (j == _objTile[k]->getIdx() && i == _objTile[k]->getIdy())
				{
					node->setColor(D2D1::ColorF::Black);
					node->setIsOpen(false);
					node->setAttribute("wall");
				}
			}
			_vTotalList.push_back(node);
		}
	}
}

void prison_5f::resetEverything()
{
	_numCount = 0;
	_vOpenList.clear();
	_vCloseList.clear();

	for (int i = 0; i < _vTotalList.size(); ++i)
	{
		_vTotalList[i]->setCostFromStart(0);
		_vTotalList[i]->setCostToGoal(0);
		_vTotalList[i]->setNumber(0);
		_vTotalList[i]->setTotalCost(0);
		_vTotalList[i]->setParentNode(NULL);
		if (_vTotalList[i]->getAttribute() != "wall") _vTotalList[i]->setIsOpen(true);
	}
}

vector<astarTile*> prison_5f::addOpenList(astarTile * currentTile)
{
	int startX = currentTile->getIdx() - 1;
	int startY = currentTile->getIdy() - 1;

	for (int i = 2; i >= 0; --i)
		//for (int i = 0; i < 3; ++i)
	{
		//// ############ ���� �������� #####################
		if (startY + i < 0)				continue;
		if (startY + i >= TILEY)		continue;
		for (int j = 2; j >= 0; --j)
			//for (int j = 0; j<3; ++j)
		{
			// ############ ���׷� ��ô ���� #####################
			if (startX + j < 0)			continue;
			if (startX + j >= TILEX)	continue;

			int checkIndex = (startY * TILEX) + startX + j + (i * TILEX);
			// startX = 13, startY = 15	-> 195
			// 1. 600 + 13 + 0 + (0 * 40) = 613 2. 600 + 13 + 1 + (0 * 40) = 614 3. 600 + 13 + 2 + (0 * 40) = 615
			// 4. 600 + 13 + 0 + (1 * 40) = 653 5. 600 + 13 + 1 + (1 * 40) = 654 6. 600 + 13 + 2 + (1 * 40) = 655
			// 7. 600 + 13 + 0 + (2 * 40) = 693 8. 600 + 13 + 1 + (2 * 40) = 693 9. 600 + 13 + 2 + (2 * 40) = 695
			if (i == 0 && j == 0) continue; // ���� ��
			if (i == 2 && j == 0) continue; // ���� �Ʒ�
			if (i == 2 && j == 2) continue; // ������ �Ʒ�
			if (i == 0 && j == 2) continue; // ������ ��
			//cout << checkIndex << endl;
			//if (checkIndex == (startX * startY - TILEY - 1)) continue;
			//if (checkIndex == (startX * startY - TILEY + 1)) continue;
			//if (checkIndex == (startX * startY + TILEY - 1)) continue;
			//if (checkIndex == (startX * startY + TILEY + 1)) continue;
			astarTile* node = _vTotalList[checkIndex];

			//����ó��!
			if (!node->getIsOpen()) continue;
			if (node->getAttribute() == "player") continue;
			if (node->getAttribute() == "wall") continue;

			// ##################���� ����#############################
			string nodeTop;
			if (startY < 0)				nodeTop = "wall";
			else nodeTop = _vTotalList[(startY * TILEX) + startX + 1]->getAttribute();
			// ������������ �������� ������ ���� �������� i==0�� i==2�Ʒ� j==0�� j==2����
			if (i == 0 && nodeTop == "wall")	continue;
			string nodeBottom;
			if (startY + 2 >= TILEY)	nodeBottom = "wall";
			else nodeBottom = _vTotalList[(startY * TILEX) + startX + (2 * TILEX) + 1]->getAttribute();
			// �Ʒ����������� ���ʾƷ��� ������ �Ʒ��� ��������
			if (i == 2 && nodeBottom == "wall")	continue;
			string nodeLeft;
			if (startX < 0)				nodeLeft = "wall";
			else nodeLeft = _vTotalList[(startY * TILEX) + startX + TILEX]->getAttribute();
			// ���ʸ��������� �������� ���ʾƷ��� ��������
			if (j == 0 && nodeLeft == "wall")	continue;
			string nodeRight;
			if (startX + 2 >= TILEX)	nodeRight = "wall";
			else nodeRight = _vTotalList[(startY * TILEX) + startX + TILEX + 2]->getAttribute();
			// �����ʸ��������� ���������� ������ �Ʒ��� ��������
			if (j == 2 && nodeRight == "wall")	continue;


			//���� Ÿ���� ��� �������ش�
			node->setParentNode(_currentTile);

			bool addObj = true;

			for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
			{
				if (*_viOpenList == node)
				{ // �ִ°Ÿ� �߰�����
					addObj = false;
					break;
				}
			}
			if (!addObj) continue;
			_vOpenList.push_back(node);
		}
		//cout << endl << endl;
	}
	return _vOpenList;
}

void prison_5f::pathFinder(astarTile * currentTile)
{//��κ���� �ſ� ���� �ϱ� ���ؼ� ������ ��κ���� ��
	float tempTotalCost = 5000;
	astarTile* tempTile = nullptr;

	bool changed = false;

	//���� �ִ� ���� ���� ���� ���ο��� ���� ���� ��θ� �̾ƾ��Ѵ�
	for (int i = 0; i < addOpenList(currentTile).size(); ++i)
	{ // ���⼭ �ֺ����� ������ ���ÿ� �������� �ѿ��¸���Ʈ�� �������°�
		_vOpenList[i]->setCostToGoal( // H�� ���(�������� �����Ÿ�)
			(abs(_enemyTile->getIdx() - _vOpenList[i]->getIdx()) +
				abs(_enemyTile->getIdy() - _vOpenList[i]->getIdy())) * 10);

		POINT center1 = _vOpenList[i]->getParentNode()->getCenter();
		POINT center2 = _vOpenList[i]->getCenter();

		_vOpenList[i]->setCostFromStart(10);
		// �����̸�(x,y �����ϳ��� ���ٸ�) 10, �ƴϸ� �밢���̴� 14�� ���

		//F = G + H �ϱ�~ 
		_vOpenList[i]->setTotalCost(			//F
			_vOpenList[i]->getCostToGoal() +	//H
			_vOpenList[i]->getCostFromStart()); //G

		//��κ��(F)�� ���� ���� �ַ� ��� �������ش�
		if (tempTotalCost > _vOpenList[i]->getTotalCost())
		{
			tempTotalCost = _vOpenList[i]->getTotalCost();
			tempTile = _vOpenList[i];
		} // ��������Ʈ�� ��κ�� ���������ְ� Ž���Ǿ� ����Ÿ�Ϸ� ����

		bool addObj = true;
		for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
		{
			if (*_viOpenList == tempTile)
			{ // ��������Ʈ�� ����Ÿ���� �ִٸ� -> ���� ��������߿����� ��밪�� ������ ����?
				addObj = false;
				changed = true;
				break;
			} // �Ұ������ �������� ������ false�� ����� �������� �ȵ���������
		}
		_vOpenList[i]->setIsOpen(false); // ����
		if (!addObj) continue;

		_vOpenList.push_back(tempTile); // �߰��ؾߵȴٰ� ������ �߰��ϴ°�
	}

	// ############## ��� ��ã���� ������ �Լ� #####################
	if (!changed)
	{// �ƹ��͵� ������ϰ� ������ ������(?) ���Ҵٸ� ������
		_stop = true;
		return;
	}

	//�����ߴٸ�
	if (tempTile->getAttribute() == "enemy")
	{
		_enemyTile->setAttribute("");
		_enemyTile->setColor(D2D1::ColorF::White);
		while (_currentTile->getParentNode() != NULL)
		{
			++_numCount;
			_currentTile->setNumber(_numCount); // ���ڸ� �������ټ� ����
			if (_numCount == 1)
			{
				_skulX = _currentTile->getIdx();
				_skulY = _currentTile->getIdy();
				_vTotalList[_skulY*TILEX + _skulX]->setAttribute("enemy");
				_vTotalList[_skulY*TILEX + _skulX]->setColor(D2D1::ColorF::Red);
				_enemyTile = _vTotalList[_skulY*TILEX + _skulX];

				_skul->setX(_enemyTile->getRect().left);			// ���ʹ� ������ X
				_skul->setY(_enemyTile->getRect().top);				// ���ʹ� ������ Y
			}
			_currentTile = _currentTile->getParentNode();
		}
		_playerTile->setNumber(_numCount + 1);
		_start = false;
		return;
	}

	_vCloseList.push_back(tempTile);

	// ����Ÿ���� ��������Ʈ�� �ְ� ���¸���Ʈ���� ����. �׸��� ����Ÿ�Ϸ� ����
	for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
	{
		if (*_viOpenList == tempTile)
		{
			_viOpenList = _vOpenList.erase(_viOpenList);
			break;
		}
	}
	_currentTile = tempTile;
}

#include "stdafx.h"
#include "garDen.h"

garDen::garDen()
{
}

garDen::~garDen()
{
}

HRESULT garDen::init()
{
	_playerTile = new astarTile;
	_enemyTile = new astarTile;
	_triggerTile = new astarTile;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		_objTile[i] = new astarTile;
	}

	MaxIndex = _numCount = b_left = b_top = 0;

	IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/Ÿ��.png", 7, 2);
	IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
	IMAGEMANAGER->AddImage("���", L"Image/mapTool/001.png");
	
	camera = Vector2(0, 0);
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

	load();

	//_boss = new boss;
	//_boss->init(11, 14);

	//_skul = new skul;
	//_skul->init(11, 14);

	_bear = new bear;
	_bear->init(11, 14);

	_p_x = camera.x;
	_p_y = camera.y;

	objectLocation();	// ī�޶� ������ �Ǿ� �������� �ڸ��� �Է��� �����ҵ�

	_setTile = false;
	_isTrigger = false;
	return S_OK;
}

void garDen::update()
{
	CAMERAMANAGER->setCamera(camera);
	cameraMove();
	//_boss->update();
	//_skul->update();
	_bear->update();
	CAMERAMANAGER->setWorldMouse(_ptMouse);

	_p_x = camera.x + WINSIZEX / 2;
	_p_y = camera.y + WINSIZEY / 2;

	playerLocation();
	//enemyLocation();

	// ��Ÿ�� (ó���� ������ �����Ⱚ�̹Ƿ� ���⼭ �ѹ��� �����Ŵ)
	if (!_setTile)
	{
		enemyLocation();
		setAstarTile();
		_setTile = true;
	}

	if (_isTrigger)
	{
		_follow_count++;
		if (_follow_count >= 10)
		{
			resetEverything();
			_currentTile = _playerTile;
			while (_numCount == 0 && !_stop) pathFinder(_currentTile);
			if (Math::GetDistance(_playerTile->getIdx(), _playerTile->getIdy(), _enemyTile->getIdx(), _enemyTile->getIdy()) < 1.5f)
			{
				_stop = true;
				return;
			}
			_follow_count = 0;
		}
	}

	RECT temp;
	if(IntersectRect(&temp, &_playerTile->getRect(), &_triggerTile->getRect()))
		_isTrigger = true;
}

void garDen::release()
{
}

void garDen::render()
{
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("���"), Vector2(720, 648));
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (KEYMANAGER->isToggleKey(VK_TAB))
			{
				CAMERAMANAGER->renderRc(_tiles[i*TILEX + j].rc,D2D1::ColorF::White,1,1);
				if (_tiles[i*TILEX + j].isCollider)
				{
					CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Red, 0.4);
				}
			}
			if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aqua, 0.5);
		}
	}

	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;
			CAMERAMANAGER->FrameRender(IMAGEMANAGER->FindImage("ObjectSample"),
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2),
				_tiles[i*TILEX + j].objFrameX, _tiles[i*TILEX + j].objFrameY);
		}
	}

	if (KEYMANAGER->isToggleKey(VK_TAB))
	{
		for (int i = 0; i < _vTotalList.size(); ++i)
		{
			_vTotalList[i]->render();
		}
	}

	//_boss->render();
	//_skul->render();
	if(_isTrigger)
		_bear->render();
}

void garDen::cameraMove()
{
	//ī�޶� �������� �� Ÿ�ϵ� �Բ� �����̵��� ������.
	//ī�޶� �����̴� ����� Ÿ�� �����̴� ������ �ݴ뿩����(ī�޶� +�� Ÿ����-����, ī�޶� -���̸� Ÿ���� +���� �����)
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		camera.y -= 5;
		CAMERAMANAGER->setRelativeY(-5.0);
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		camera.y += 5;
		CAMERAMANAGER->setRelativeY(5.0);
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		CAMERAMANAGER->setRelativeX(5.0);
		camera.x += 5;
	}
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		camera.x -= 5;
		CAMERAMANAGER->setRelativeX(-5.0);
	}
}

void garDen::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("save/save2.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;

	CAMERAMANAGER->setCamera(camera);
	CloseHandle(file);
}

void garDen::playerLocation()	// �÷��̾� Ÿ�� ����
{
	p_left = _p_x / TILESIZE;				// �÷��̾� �ִ� Ÿ���ڸ�
	p_top = _p_y / TILESIZE;				// �÷��̾� �ִ� Ÿ���ڸ�
	_playerTile->init(p_left, p_top);		// astarŸ�Ϸ� �������� ����
	_playerTile->setAttribute("player");	// astarŸ�Ϸ� �÷��̾� ����
}

void garDen::enemyLocation()	// �� Ÿ�� ����
{
	//b_left = _boss->getRect().left / TILESIZE;			// ���ʹ� �ִ� Ÿ���ڸ�
	//b_top = _boss->getRect().top / TILESIZE;				// ���ʹ� �ִ� Ÿ���ڸ�
	//b_left = _skul->getRect().left / TILESIZE;			// ���ʹ� �ִ� Ÿ���ڸ�
	//b_top = _skul->getRect().top / TILESIZE;				// ���ʹ� �ִ� Ÿ���ڸ�
	b_left = _bear->getRect().left / TILESIZE;				// ���ʹ� �ִ� Ÿ���ڸ�
	b_top = _bear->getRect().top / TILESIZE;				// ���ʹ� �ִ� Ÿ���ڸ�
	_enemyTile->init(b_left, b_top);						// astarŸ�Ϸ� �������� ����
	_enemyTile->setAttribute("enemy");						// astarŸ�Ϸ� ���ʹ� ����
}

void garDen::objectLocation()	// ������Ʈ Ÿ�� ����
{
	for (int i = 0; i < (TILEX * TILEY); i++)
	{
		if (_tiles[i].isCollider == true)
		{
			_objTile[MaxIndex]->init(_tiles[i].rc.left / TILESIZE, _tiles[i].rc.top / TILESIZE);
			MaxIndex++;
		}
		if (_tiles[i].terrain == TR_TRIGGER)	// Ʈ���� Ÿ�� ����
		{
			cout << i << endl;
			_triggerTile->init(_tiles[i].rc.left / TILESIZE, _tiles[i].rc.top / TILESIZE);
		}
	}

	for (int i = 0; i < MaxIndex; i++)
	{
		_objTile[i]->setAttribute("wall");
	}
}

void garDen::getTrigger()
{
}

void garDen::setAstarTile()
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

void garDen::resetEverything()
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

vector<astarTile*> garDen::addOpenList(astarTile * currentTile)
{
	int startX = currentTile->getIdx() - 1;
	int startY = currentTile->getIdy() - 1;

	for (int i = 0; i < 3; ++i)
	{
		//// ############ ���� �������� #####################
		if (startY + i < 0)				continue;
		if (startY + i >= TILEY)		continue;
		for (int j = 0; j < 3; ++j)
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

void garDen::pathFinder(astarTile * currentTile)
{
	//��κ���� �ſ� ���� �ϱ� ���ؼ� ������ ��κ���� ��
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

		_vOpenList[i]->setCostFromStart((center1.x == center2.x ||
			center1.y == center2.y) ? 14 : 10);
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
				b_left = _currentTile->getIdx();
				b_top = _currentTile->getIdy();
				_vTotalList[b_top*TILEX + b_left]->setAttribute("enemy");
				_vTotalList[b_top*TILEX + b_left]->setColor(D2D1::ColorF::Red);
				_enemyTile = _vTotalList[b_top*TILEX + b_left];

				//_boss->setX(_enemyTile->getRect().left);					// ���ʹ� ������ X
				//_boss->setY(_enemyTile->getRect().top);					// ���ʹ� ������ Y
				//_skul->setX(_enemyTile->getRect().left);					// ���ʹ� ������ X
				//_skul->setY(_enemyTile->getRect().top);					// ���ʹ� ������ Y
				_bear->setX(_enemyTile->getRect().left);					// ���ʹ� ������ X
				_bear->setY(_enemyTile->getRect().top);						// ���ʹ� ������ Y
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

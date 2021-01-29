#include "stdafx.h"
#include "garDen.h"
#include "boss.h"

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
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		_objTile[i] = new astarTile;
	}

	MaxIndex = _numCount = 0;

	IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/Ÿ��.png", 7, 2);
	IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
	IMAGEMANAGER->AddImage("���", L"Image/mapTool/001.png");
	
	camera = Vector2(0, 0);
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

	load();

	_boss = new boss;
	_boss->init(10, 13);

	_p_x = camera.x;
	_p_y = camera.y;

	objectLocation();	// ī�޶� ������ �Ǿ� �������� �ڸ��� �Է��� �����ҵ�

	_setTile = false;

	return S_OK;
}

void garDen::update()
{
	CAMERAMANAGER->setCamera(camera);
	cameraMove();
	_boss->update();
	CAMERAMANAGER->setWorldMouse(_ptMouse);

	//_p_x = camera.x;
	//_p_y = camera.y;
	_p_x = camera.x + WINSIZEX / 2;
	_p_y = camera.y + WINSIZEY / 2;

	playerLocation();
	enemyLocation();

	// ��Ÿ�� (ó���� ������ �����Ⱚ�̹Ƿ� ���⼭ �ѹ��� �����Ŵ)
	if (!_setTile)
	{
		setAstarTile();
		_setTile = true;
	}

	_follow_count++;

	// �ð��� �Ǹ� ���� �i�ƿ��� ������ astar���� ����(�н����δ��� ���� ���� �ϴ� ���� �߿�)
	if (_follow_count >= 20)
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

	_boss->render();
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
	//cout << _tiles->camera.x;
	CAMERAMANAGER->setCamera(camera);
	CloseHandle(file);
}

void garDen::playerLocation()
{
	p_left = _p_x / TILESIZE;				// �÷��̾� �ִ� Ÿ���ڸ�
	p_top = _p_y / TILESIZE;				// �÷��̾� �ִ� Ÿ���ڸ�
	_playerTile->init(p_left, p_top);		// astarŸ�Ϸ� �������� ����
	_playerTile->setAttribute("player");	// astarŸ�Ϸ� �÷��̾� ����

	//cout << "x : " << _playerTile->getIdx() << " y : " << _playerTile->getIdy() << endl;	// ���� �÷��̾� Ÿ���� ����ִ°�
}

void garDen::enemyLocation()
{
	b_left = _boss->getRect().left / TILESIZE;	// ���ʹ� �ִ� Ÿ���ڸ�
	b_top = _boss->getRect().top / TILESIZE;	// ���ʹ� �ִ� Ÿ���ڸ�
	_enemyTile->init(b_left, b_top);			// astarŸ�Ϸ� �������� ����
	_enemyTile->setAttribute("enemy");			// astarŸ�Ϸ� ���ʹ� ����

	//cout << "x : " << _enemyTile->getIdx() << " y : " << _enemyTile->getIdy() << endl;	// ���� ���ʹ� Ÿ���� ����ִ°�
}

void garDen::objectLocation()	// ī�޶� ������ �Ǿ� �������� �ڸ��� �Է��� �����ҵ�
{
	for (int i = 0; i < (TILEX * TILEY); i++)
	{
		if (_tiles[i].isCollider == true)
		{
			_objTile[MaxIndex]->init(_tiles[i].rc.left / TILESIZE, _tiles[i].rc.top / TILESIZE);
			MaxIndex++;
		}
	}

	for (int i = 0; i < MaxIndex; i++)
	{
		_objTile[i]->setAttribute("wall");
	}

	cout << MaxIndex << endl;
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
				_enemyTile->setColor(D2D1::ColorF::Blue);			// ���ʹ� Ÿ�� ���� - ��ǻ� ���п� �ǹ̾�����
				_enemyTile->setIsOpen(true);					// ���¸���Ʈ�ΰ�?
				_vTotalList.push_back(_enemyTile);				// ��Ż����Ʈ�� �ֱ�
				continue;
			}

			//�� �� Ÿ��
			astarTile* node = new astarTile;
			node->init(j, i);
			node->setIsOpen(true);
			node->setAttribute("null");

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
	}
	return _vOpenList;
}

void garDen::pathFinder(astarTile * currentTile)
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

		_vOpenList[i]->setCostFromStart((center1.x == center2.x ||
			center1.y == center2.y) ? 10 : 14);
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
		//_enemyTile->setColor(RGB(250, 150, 0));
		while (_currentTile->getParentNode() != NULL)
		{
			++_numCount;
			_currentTile->setNumber(_numCount); // ���ڸ� �������ټ� ����
			if (_numCount == 1)
			{
				b_left = _currentTile->getIdx();
				b_top = _currentTile->getIdy();
				_vTotalList[b_top*TILEX + b_left]->setAttribute("enemy");
				//_vTotalList[b_top*TILEX + b_left]->setColor(RGB(0, 255, 0));
				_enemyTile = _vTotalList[b_top*TILEX + b_left];
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

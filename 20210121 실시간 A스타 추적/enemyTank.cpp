#include "stdafx.h"
#include "enemyTank.h"

#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")

HRESULT enemyTank::init()
{
	_direction = ENEMYTANKDIRECTION_LEFT;

	_image = new image;
	_image->init("tank.bmp", 0, 0, 256, 128, 8, 4, true, RGB(255, 0, 255));

	_speed = 100.0f;
	_x = 300;
	_y = 300;
	_rc = RectMake(_x, _y, 28, 28);
	_count = _start = _stop = _numCount = 0;

	_playerTile = new tile;
	_emTile = new tile;
	for (int i = 0; i < 400; i++)
	{
		_objTile[i] = new tile;
	}

	_px = 0;
	_py = 0;
	_ex = 0;
	_ey = 0;
	_setTile = false;
	
	return S_OK;
}

void enemyTank::release()
{
}

void enemyTank::update()
{
	// �÷��̾� ��ġ ��� ����
	_px = _player->getCurrentTileX();
	_py = _player->getCurrentTileY();

	// ���ʹ� ��ġ ��� ����
	_ex = _rc.left / TILESIZE;
	_ey = _rc.top / TILESIZE;

	// ��Ÿ�� (ó���� ������ �����Ⱚ�̹Ƿ� ���⼭ �ѹ��� �����Ŵ)
	if (!_setTile)
	{
		setTiles(_px, _px, _ex, _ex);
		_setTile = true;
	}

	// �÷��̾� ��ġ ��� ����
	_playerTile->init(_px, _py);
	_playerTile->setAttribute("start");

	// ��ũ ���� ��ġ�� �̿��� �̹��� ���� ����
	tankDirection();

	_follow_count++;

	// �ð��� �Ǹ� ���� �i�ƿ��� ������ astar���� ����(�н����δ��� ���� ���� �ϴ� ���� �߿�)
	if (_follow_count >= 10)
	{
		resetEverything();
		_currentTile = _playerTile;
		while (_numCount == 0 && !_stop) pathFinder(_currentTile);
		if (getDistance(_playerTile->getIdx(), _playerTile->getIdy(), _emTile->getIdx(), _emTile->getIdy()) < 1.5f)
		{
			_stop = true;
			return;
		}
		_follow_count = 0;
	}
}

void enemyTank::render()
{
	_image->frameRender(getMemDC(), _rc.left, _rc.top);

	if (KEYMANAGER->isToggleKey(VK_TAB))
	{
		for (int i = 0; i < _vTotalList.size(); ++i)
		{
			_vTotalList[i]->render();
		}
		if (_stop)
		{ // ��θ� �� ã�Ҵٸ�
			for (int i = 0; i < _vTotalList.size(); ++i)
			{
				if (_vTotalList[i]->getAttribute() == "start") _vTotalList[i]->setColor(RGB(255, 0, 0));
				if (_vTotalList[i]->getAttribute() == "end") _vTotalList[i]->setColor(RGB(255, 0, 0));
			}
		}
		char str[128]; // Ŀ��ƮŸ���� ���� ����� ǥ���Ϸ���
		ZeroMemory(str, 126);
		sprintf_s(str, "cur");
		TextOut(getMemDC(), _currentTile->getRect().left, _currentTile->getRect().top, str, strlen(str));
	}
}

void enemyTank::setTankPosition()
{
	_rc = _tankMap->getTile()[_tankMap->getPosFirst()].rc;

	_x = (_rc.left + _rc.right) / 2;
	_y = (_rc.top + _rc.bottom) / 2;
}

void enemyTank::tankDirection()
{
	switch (_direction)
	{
		case ENEMYTANKDIRECTION_LEFT:
			_image->setFrameX(0); _image->setFrameY(3);
		break;
		case ENEMYTANKDIRECTION_UP:
			_image->setFrameX(0); _image->setFrameY(0);
		break;
		case ENEMYTANKDIRECTION_RIGHT:
			_image->setFrameX(0); _image->setFrameY(2);
		break;
		case ENEMYTANKDIRECTION_DOWN:
			_image->setFrameX(0); _image->setFrameY(1);
		break;
	}

	RECT previous_rc;
	previous_rc = _rc;

	_rc.left = _emTile->getRect().left;
	_rc.top = _emTile->getRect().top;
	_rc.right = _rc.left + 28;
	_rc.bottom = _rc.top + 28;

	if (previous_rc.left > _rc.left)
		_direction = ENEMYTANKDIRECTION_LEFT;
	if (previous_rc.right < _rc.right)
		_direction = ENEMYTANKDIRECTION_RIGHT;
	if (previous_rc.top > _rc.top)
		_direction = ENEMYTANKDIRECTION_UP;
	if (previous_rc.bottom < _rc.bottom)
		_direction = ENEMYTANKDIRECTION_DOWN;
}

void enemyTank::setTiles(int px, int py, int ex, int ey)
{	
	//���� Ÿ��. �Ӽ��� "start"�� ������
	_playerTile->init(px, py);
	_playerTile->setAttribute("start");

	//�� Ÿ��. �Ӽ��� "end"�� ������
	_emTile->init(ex+1, ey+1);
	_emTile->setAttribute("end");
	
	// Ÿ�ϸʿ��� ��ֹ��μ� ��� ���ϴ� Ÿ���� �����ͼ� ������ƮŸ�Ϸ� ����
	int MaxIndex = 0;
	for (int i = 0; i < 400; i++)
	{
		if (_tankMap->getAttribute()[i] & ATTR_UNMOVE == ATTR_UNMOVE)
		{
			_objTile[MaxIndex]->init(_tankMap->getTile()[i].rc.left / TILESIZE, _tankMap->getTile()[i].rc.top / TILESIZE);
			MaxIndex++;
		}
	}

	for (int i = 0; i < MaxIndex; i++)
	{
		_objTile[i]->setAttribute("wall");
	}

	//���� Ÿ���� ����Ÿ�Ϸ�ٰ�
	_currentTile = _playerTile;

	for (int i = 0; i < TILEY; ++i)
	{ // ��Ż����Ʈ�� ������ �����Ͽ� ����
		for (int j = 0; j < TILEX; ++j)
		{
			//���� Ÿ��
			if (j == _playerTile->getIdx() && i == _playerTile->getIdy())
			{
				_playerTile->setColor(RGB(255, 0, 255));
				_playerTile->setIsOpen(true);
				_vTotalList.push_back(_playerTile);
				continue;

			}
			//�� Ÿ��
			if (j == _emTile->getIdx() && i == _emTile->getIdy())
			{
				_emTile->setColor(RGB(10, 120, 55));
				_emTile->setIsOpen(true);
				_vTotalList.push_back(_emTile);
				continue;
			}

			//�� �� Ÿ��
			tile* node = new tile;
			node->init(j, i);
			node->setIsOpen(true);

			// �� �� Ÿ�Ͽ��� ��ֹ��̸� ���� ���� �� ��� �Ұ����ϰ� ����
			for (int k = 0; k < MaxIndex; k++)
			{
				if (j == _objTile[k]->getIdx() && i == _objTile[k]->getIdy())
				{
					node->setColor(RGB(200, 200, 200));
					node->setIsOpen(false);
					node->setAttribute("wall");
				}
			}

			//��Ż���Ϳ� Ÿ���� �� �������
			_vTotalList.push_back(node);
		}
	}
}

vector<tile*> enemyTank::addOpenList(tile * currentTile)
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
			tile* node = _vTotalList[checkIndex];

			//����ó��!
			if (!node->getIsOpen()) continue;
			if (node->getAttribute() == "start") continue;
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

void enemyTank::resetEverything()
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

void enemyTank::pathFinder(tile * currentTile)
{
	//��κ���� �ſ� ���� �ϱ� ���ؼ� ������ ��κ���� ��
	float tempTotalCost = 5000;
	tile* tempTile = nullptr;

	bool changed = false;

	//���� �ִ� ���� ���� ���� ���ο��� ���� ���� ��θ� �̾ƾ��Ѵ�
	for (int i = 0; i < addOpenList(currentTile).size(); ++i)
	{ // ���⼭ �ֺ����� ������ ���ÿ� �������� �ѿ��¸���Ʈ�� �������°�
		_vOpenList[i]->setCostToGoal( // H�� ���(�������� �����Ÿ�)
			(abs(_emTile->getIdx() - _vOpenList[i]->getIdx()) +
				abs(_emTile->getIdy() - _vOpenList[i]->getIdy())) * 10);

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
	if (tempTile->getAttribute() == "end")
	{
		_emTile->setAttribute("");
		_emTile->setColor(RGB(250, 150, 0));
		while (_currentTile->getParentNode() != NULL)
		{
			++_numCount;
			_currentTile->setNumber(_numCount); // ���ڸ� �������ټ� ����
			if (_numCount == 1)
			{
				_ex = _currentTile->getIdx();
				_ey = _currentTile->getIdy();
				_vTotalList[_ey*TILEX + _ex]->setAttribute("end");
				_vTotalList[_ey*TILEX + _ex]->setColor(RGB(0, 255, 0));
				_emTile = _vTotalList[_ey*TILEX + _ex];
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

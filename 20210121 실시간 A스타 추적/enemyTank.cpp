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
	// 플레이어 위치 계속 적용
	_px = _player->getCurrentTileX();
	_py = _player->getCurrentTileY();

	// 에너미 위치 계속 적용
	_ex = _rc.left / TILESIZE;
	_ey = _rc.top / TILESIZE;

	// 셋타일 (처음에 변수가 쓰레기값이므로 여기서 한번만 적용시킴)
	if (!_setTile)
	{
		setTiles(_px, _px, _ex, _ex);
		_setTile = true;
	}

	// 플레이어 위치 계속 적용
	_playerTile->init(_px, _py);
	_playerTile->setAttribute("start");

	// 탱크 이전 위치를 이용해 이미지 방향 설정
	tankDirection();

	_follow_count++;

	// 시간이 되면 나를 쫒아오는 실질적 astar적용 업뎃(패스파인더를 지속 적용 하는 것이 중요)
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
		{ // 경로를 못 찾았다면
			for (int i = 0; i < _vTotalList.size(); ++i)
			{
				if (_vTotalList[i]->getAttribute() == "start") _vTotalList[i]->setColor(RGB(255, 0, 0));
				if (_vTotalList[i]->getAttribute() == "end") _vTotalList[i]->setColor(RGB(255, 0, 0));
			}
		}
		char str[128]; // 커런트타일이 지금 어딘지 표시하려고
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
	//시작 타일. 속성은 "start"로 정했음
	_playerTile->init(px, py);
	_playerTile->setAttribute("start");

	//끝 타일. 속성은 "end"로 정했음
	_emTile->init(ex+1, ey+1);
	_emTile->setAttribute("end");
	
	// 타일맵에서 장애물로서 통과 못하는 타일을 가져와서 오브젝트타일로 정함
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

	//현재 타일은 시작타일루다가
	_currentTile = _playerTile;

	for (int i = 0; i < TILEY; ++i)
	{ // 토탈리스트에 색깔을 지정하여 넣음
		for (int j = 0; j < TILEX; ++j)
		{
			//시작 타일
			if (j == _playerTile->getIdx() && i == _playerTile->getIdy())
			{
				_playerTile->setColor(RGB(255, 0, 255));
				_playerTile->setIsOpen(true);
				_vTotalList.push_back(_playerTile);
				continue;

			}
			//끝 타일
			if (j == _emTile->getIdx() && i == _emTile->getIdy())
			{
				_emTile->setColor(RGB(10, 120, 55));
				_emTile->setIsOpen(true);
				_vTotalList.push_back(_emTile);
				continue;
			}

			//그 외 타일
			tile* node = new tile;
			node->init(j, i);
			node->setIsOpen(true);

			// 그 외 타일에서 장애물이면 색상 변경 및 통과 불가능하게 변경
			for (int k = 0; k < MaxIndex; k++)
			{
				if (j == _objTile[k]->getIdx() && i == _objTile[k]->getIdy())
				{
					node->setColor(RGB(200, 200, 200));
					node->setIsOpen(false);
					node->setAttribute("wall");
				}
			}

			//토탈벡터에 타일을 다 담아주자
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
		//// ############ 벡터 안터지게 #####################
		if (startY + i < 0)				continue;
		if (startY + i >= TILEY)		continue;
		for (int j = 0; j < 3; ++j)
		{
			// ############ 신항로 개척 방지 #####################
			if (startX + j < 0)			continue;
			if (startX + j >= TILEX)	continue;

			int checkIndex = (startY * TILEX) + startX + j + (i * TILEX);
			tile* node = _vTotalList[checkIndex];

			//예외처리!
			if (!node->getIsOpen()) continue;
			if (node->getAttribute() == "start") continue;
			if (node->getAttribute() == "wall") continue;

			// ##################월담 방지#############################
			string nodeTop;
			if (startY < 0)				nodeTop = "wall";
			else nodeTop = _vTotalList[(startY * TILEX) + startX + 1]->getAttribute();
			// 위막혀있으면 왼쪽위와 오른쪽 위를 못가게함 i==0위 i==2아래 j==0왼 j==2오른
			if (i == 0 && nodeTop == "wall")	continue;
			string nodeBottom;
			if (startY + 2 >= TILEY)	nodeBottom = "wall";
			else nodeBottom = _vTotalList[(startY * TILEX) + startX + (2 * TILEX) + 1]->getAttribute();
			// 아래막혀있으면 왼쪽아래와 오른쪽 아래를 못가게함
			if (i == 2 && nodeBottom == "wall")	continue;
			string nodeLeft;
			if (startX < 0)				nodeLeft = "wall";
			else nodeLeft = _vTotalList[(startY * TILEX) + startX + TILEX]->getAttribute();
			// 왼쪽막혀있으면 왼쪽위와 왼쪽아래를 못가게함
			if (j == 0 && nodeLeft == "wall")	continue;
			string nodeRight;
			if (startX + 2 >= TILEX)	nodeRight = "wall";
			else nodeRight = _vTotalList[(startY * TILEX) + startX + TILEX + 2]->getAttribute();
			// 오른쪽막혀있으면 오른쪽위와 오른쪽 아래로 못가게함
			if (j == 2 && nodeRight == "wall")	continue;


			//현재 타일을 계속 갱신해준다
			node->setParentNode(_currentTile);

			bool addObj = true;

			for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
			{
				if (*_viOpenList == node)
				{ // 있는거면 추가안함
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
	//경로비용을 매우 쉽게 하기 위해서 임의의 경로비용을 둠
	float tempTotalCost = 5000;
	tile* tempTile = nullptr;

	bool changed = false;

	//갈수 있는 길을 담은 벡터 내부에서 가장 빠른 경로를 뽑아야한다
	for (int i = 0; i < addOpenList(currentTile).size(); ++i)
	{ // 여기서 주변방향 넣음과 동시에 리턴으로 총오픈리스트를 가져오는거
		_vOpenList[i]->setCostToGoal( // H를 계산(끝점과의 수직거리)
			(abs(_emTile->getIdx() - _vOpenList[i]->getIdx()) +
				abs(_emTile->getIdy() - _vOpenList[i]->getIdy())) * 10);

		POINT center1 = _vOpenList[i]->getParentNode()->getCenter();
		POINT center2 = _vOpenList[i]->getCenter();

		_vOpenList[i]->setCostFromStart((center1.x == center2.x ||
			center1.y == center2.y) ? 10 : 14);
		// 직선이면(x,y 둘중하나가 같다면) 10, 아니면 대각선이니 14의 비용

		//F = G + H 니까~ 
		_vOpenList[i]->setTotalCost(			//F
			_vOpenList[i]->getCostToGoal() +	//H
			_vOpenList[i]->getCostFromStart()); //G

		//경로비용(F)이 가장 작은 애로 계속 갱신해준다
		if (tempTotalCost > _vOpenList[i]->getTotalCost())
		{
			tempTotalCost = _vOpenList[i]->getTotalCost();
			tempTile = _vOpenList[i];
		} // 열린리스트중 경로비용 가장작은애가 탐색되어 템프타일로 들어옴

		bool addObj = true;
		for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
		{
			if (*_viOpenList == tempTile)
			{ // 열린리스트에 템프타일이 있다면 -> 새로 열린놈들중에서도 비용값이 작은게 없다?
				addObj = false;
				changed = true;
				break;
			} // 불값만들고 포문에서 잡히면 false로 만드니 이중포문 안돌릴수있음
		}
		_vOpenList[i]->setIsOpen(false); // 닫음
		if (!addObj) continue;

		_vOpenList.push_back(tempTile); // 추가해야된다고 했으니 추가하는거
	}

	// ############## 경로 못찾으면 끝내는 함수 #####################
	if (!changed)
	{// 아무것도 변경안하고 포문이 무사히(?) 돌았다면 끝낸다
		_stop = true;
		return;
	}

	//도착했다면
	if (tempTile->getAttribute() == "end")
	{
		_emTile->setAttribute("");
		_emTile->setColor(RGB(250, 150, 0));
		while (_currentTile->getParentNode() != NULL)
		{
			++_numCount;
			_currentTile->setNumber(_numCount); // 숫자를 지정해줄수 있음
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

	// 템프타일을 닫힌리스트에 넣고 오픈리스트에선 빼줌. 그리고 현재타일로 해줌
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

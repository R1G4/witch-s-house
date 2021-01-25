#include "stdafx.h"
#include "aStarTest.h"


aStarTest::aStarTest()
{
}


aStarTest::~aStarTest()
{
}

HRESULT aStarTest::init()
{
	_count = _start = _stop = _numCount = 0;
	_startTile = new tile;
	_endTile = new tile;
	_px = 4;
	_py = 12;
	_ex = 20;
	_ey = 12;
	setTiles(_px, _py, _ex, _ey);

	return S_OK;
}

void aStarTest::setTiles(int px, int py, int ex, int ey)
{
	//시작 타일. 속성은 "start"로 정했음
	_startTile->init(px, py);
	_startTile->setAttribute("start");

	//끝 타일. 속성은 "end"로 정했음
	_endTile->init(ex, ey);
	_endTile->setAttribute("end");

	//현재 타일은 시작타일루다가
	_currentTile = _startTile;

	for (int i = 0; i < TILENUMY; ++i)
	{ // 토탈리스트에 색깔을 지정하여 넣음
		for (int j = 0; j < TILENUMX; ++j)
		{
			//시작 타일
			if (j == _startTile->getIdx() && i == _startTile->getIdy())
			{
				_startTile->setColor(RGB(255, 0, 255));
				_startTile->setIsOpen(true);
				_vTotalList.push_back(_startTile);
				continue;
			}
			//끝 타일
			if (j == _endTile->getIdx() && i == _endTile->getIdy())
			{
				_endTile->setColor(RGB(10, 120, 55));
				_endTile->setIsOpen(true);
				_vTotalList.push_back(_endTile);
				continue;
			}

			//그 외 타일
			tile* node = new tile;
			node->init(j, i);
			node->setIsOpen(true);

			//토탈벡터에 타일을 다 담아주자
			_vTotalList.push_back(node);
		}
	}

}

vector<tile*> aStarTest::addOpenList(tile* currentTile)
{
	int startX = currentTile->getIdx() - 1;
	int startY = currentTile->getIdy() - 1;

	for (int i = 0; i < 3; ++i)
	{
		//// ############ 벡터 안터지게 #####################
		if (startY + i < 0)				continue;
		if (startY + i >= TILENUMY)		continue;
		for (int j = 0; j < 3; ++j)
		{
			// ############ 신항로 개척 방지 #####################
			if (startX + j < 0)			continue;
			if (startX + j >= TILENUMX)	continue;

			int checkIndex = (startY * TILENUMX) + startX + j + (i * TILENUMX);
			tile* node = _vTotalList[checkIndex];

			//예외처리!
			if (!node->getIsOpen()) continue;
			if (node->getAttribute() == "start") continue;
			if (node->getAttribute() == "wall") continue;

			// ##################월담 방지#############################
			string nodeTop;
			if (startY < 0)				nodeTop = "wall";
			else nodeTop = _vTotalList[(startY * TILENUMX) + startX + 1]->getAttribute();
			// 위막혀있으면 왼쪽위와 오른쪽 위를 못가게함 i==0위 i==2아래 j==0왼 j==2오른
			if (i == 0 && nodeTop == "wall")	continue;
			string nodeBottom;
			if (startY + 2 >= TILENUMY)	nodeBottom = "wall";
			else nodeBottom = _vTotalList[(startY * TILENUMX) + startX + (2 * TILENUMX) + 1]->getAttribute();
			// 아래막혀있으면 왼쪽아래와 오른쪽 아래를 못가게함
			if (i == 2 && nodeBottom == "wall")	continue;			
			string nodeLeft;
			if (startX < 0)				nodeLeft = "wall";
			else nodeLeft = _vTotalList[(startY * TILENUMX) + startX + TILENUMX]->getAttribute();
			// 왼쪽막혀있으면 왼쪽위와 왼쪽아래를 못가게함
			if (j == 0 && nodeLeft == "wall")	continue;
			string nodeRight;
			if (startX + 2 >= TILENUMX)	nodeRight = "wall";
			else nodeRight = _vTotalList[(startY * TILENUMX) + startX + TILENUMX + 2]->getAttribute();
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

void aStarTest::resetEverything()
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

void aStarTest::pathFinder(tile* currentTile)
{
	//경로비용을 매우 쉽게 하기 위해서 임의의 경로비용을 둠
	float tempTotalCost = 5000;
	tile* tempTile = nullptr;

	bool changed = false;

	//갈수 있는 길을 담은 벡터 내부에서 가장 빠른 경로를 뽑아야한다
	for (int i = 0; i < addOpenList(currentTile).size(); ++i)
	{ // 여기서 주변방향 넣음과 동시에 리턴으로 총오픈리스트를 가져오는거
		_vOpenList[i]->setCostToGoal( // H를 계산(끝점과의 수직거리)
			(abs(_endTile->getIdx() - _vOpenList[i]->getIdx()) +
				abs(_endTile->getIdy() - _vOpenList[i]->getIdy())) * 10);

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
		_endTile->setAttribute("");
		_endTile->setColor(RGB(250, 150, 0));
		while (_currentTile->getParentNode() != NULL)
		{
			++_numCount;
			_currentTile->setNumber(_numCount); // 숫자를 지정해줄수 있음
			if (_numCount == 1)
			{
				_ex = _currentTile->getIdx();
				_ey = _currentTile->getIdy();
				_vTotalList[_ey*TILENUMX + _ex]->setAttribute("end");
				_vTotalList[_ey*TILENUMX + _ex]->setColor(RGB(0, 255, 0));
				_endTile = _vTotalList[_ey*TILENUMX + _ex];
			}
			_currentTile = _currentTile->getParentNode();
		}
		_startTile->setNumber(_numCount + 1);
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


	//요렇게 함수 내부에서 자기 자신을 또 호출하는 것이 재귀함수
	//호출될때 마다 소량의 메모리 스택이 쌓이는데
	//이게 약 1.2메가 이상 넘어가면 터지고야 만드아
	//그것이 바로 스택 오버 플로우 라고 함
	//그리고 재귀는 좀 느림 속도도 
	//쓰는 이유는 코드가 많이 줄고 깔끔해지니까 
	//디폴트가 1.2메가 란거지 명령어를 통해서 그 메모리 스택량을 늘릴수있음.
	//pathFinder(_currentTile);
}

void aStarTest::release()
{
}

void aStarTest::update()
{
	//플레이어 움직임
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
	{
		if (_vTotalList[_py*TILENUMX + _px - 1]->getAttribute() == "wall") return;
		_startTile->setAttribute("");
		_startTile->setColor(RGB(250, 150, 0));
		_px--;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
	{
		if (_vTotalList[_py*TILENUMX + _px + 1]->getAttribute() == "wall") return;
		_vTotalList[_py*TILENUMX + _px]->setAttribute("");
		_vTotalList[_py*TILENUMX + _px]->setColor(RGB(250, 150, 0));
		_px++;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		if (_vTotalList[(_py - 1)*TILENUMX + _px]->getAttribute() == "wall") return;
		_startTile->setAttribute("");
		_startTile->setColor(RGB(250, 150, 0));
		_py--;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		if (_vTotalList[(_py + 1)*TILENUMX + _px]->getAttribute() == "wall") return;
		_startTile->setAttribute("");
		_startTile->setColor(RGB(250, 150, 0));
		_py++;
	}

	_vTotalList[_py*TILENUMX + _px]->setAttribute("start");
	_vTotalList[_py*TILENUMX + _px]->setColor(RGB(255, 0, 255));
	_startTile = _vTotalList[_py*TILENUMX + _px];

	_follow_count++;

	// 시간이 되면 나를 쫒아오는 실질적 astar적용 업뎃(패스파인더를 지속적용하는것이 중요)
	if (_follow_count >= 50)
	{
		resetEverything();
		_currentTile = _startTile;
		while (_numCount == 0 && !_stop) pathFinder(_currentTile);
		if (getDistance(_startTile->getIdx(), _startTile->getIdy(), _endTile->getIdx(), _endTile->getIdy()) < 1.5f)
		{
			_stop = true;
			return;
		}
		_follow_count = 0;
	}

	// 장애물 적용
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{ // 갈수없는 타일로 바꿈
		for (int i = 0; i < _vTotalList.size(); ++i)
		{
			if (PtInRect(&_vTotalList[i]->getRect(), _ptMouse))
			{
				if (_vTotalList[i]->getAttribute() == "start") continue;
				if (_vTotalList[i]->getAttribute() == "end") continue;

				_vTotalList[i]->setIsOpen(false);
				_vTotalList[i]->setAttribute("wall");
				_vTotalList[i]->setColor(RGB(230, 140, 200));

				break;
			}
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
	{ 
		// 갈수있는 타일로 바꿈(실행취소용)
		for (int i = 0; i < _vTotalList.size(); ++i)
		{
			if (PtInRect(&_vTotalList[i]->getRect(), _ptMouse))
			{
				if (_vTotalList[i]->getAttribute() == "start") continue;
				if (_vTotalList[i]->getAttribute() == "end") continue;

				_vTotalList[i]->setIsOpen(true);
				_vTotalList[i]->setAttribute("");
				_vTotalList[i]->setColor(RGB(250, 150, 0));

				break;
			}
		}
	}
}

void aStarTest::render()
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

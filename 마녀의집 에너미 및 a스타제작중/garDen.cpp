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

	IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/타일.png", 7, 2);
	IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
	IMAGEMANAGER->AddImage("배경", L"Image/mapTool/001.png");
	
	camera = Vector2(0, 0);
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

	load();

	_boss = new boss;
	_boss->init(10, 13);

	_p_x = camera.x;
	_p_y = camera.y;

	objectLocation();	// 카메라 적용이 되야 정상적인 자리에 입력이 가능할듯

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

	// 셋타일 (처음에 변수가 쓰레기값이므로 여기서 한번만 적용시킴)
	if (!_setTile)
	{
		setAstarTile();
		_setTile = true;
	}

	_follow_count++;

	// 시간이 되면 나를 쫒아오는 실질적 astar적용 업뎃(패스파인더를 지속 적용 하는 것이 중요)
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
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경"), Vector2(720, 648));
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
	//카메라를 움직였을 때 타일도 함께 움직이도록 세팅함.
	//카메라 움직이는 방향과 타일 움직이는 방향은 반대여야함(카메라가 +면 타일은-값을, 카메라가 -값이면 타일은 +값을 줘야함)
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
	p_left = _p_x / TILESIZE;				// 플레이어 있는 타일자리
	p_top = _p_y / TILESIZE;				// 플레이어 있는 타일자리
	_playerTile->init(p_left, p_top);		// astar타일로 지속적인 적용
	_playerTile->setAttribute("player");	// astar타일로 플레이어 적용

	//cout << "x : " << _playerTile->getIdx() << " y : " << _playerTile->getIdy() << endl;	// 현재 플레이어 타일이 어디있는가
}

void garDen::enemyLocation()
{
	b_left = _boss->getRect().left / TILESIZE;	// 에너미 있는 타일자리
	b_top = _boss->getRect().top / TILESIZE;	// 에너미 있는 타일자리
	_enemyTile->init(b_left, b_top);			// astar타일로 지속적인 적용
	_enemyTile->setAttribute("enemy");			// astar타일로 에너미 적용

	//cout << "x : " << _enemyTile->getIdx() << " y : " << _enemyTile->getIdy() << endl;	// 현재 에너미 타일이 어디있는가
}

void garDen::objectLocation()	// 카메라 적용이 되야 정상적인 자리에 입력이 가능할듯
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
	//현재 타일은 시작타일루다가
	_currentTile = _playerTile;

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			if (j == _playerTile->getIdx() && i == _playerTile->getIdy())
			{
				_playerTile->setColor(D2D1::ColorF::Green);		// 플레이어 타일 색상 - 사실상 구분용 의미없을듯
				_playerTile->setIsOpen(true);					// 오픈리스트인가?
				_vTotalList.push_back(_playerTile);				// 토탈리스트에 넣기
				continue;
			}

			if (j == _enemyTile->getIdx() && i == _enemyTile->getIdy())
			{
				_enemyTile->setColor(D2D1::ColorF::Blue);			// 에너미 타일 색상 - 사실상 구분용 의미없을듯
				_enemyTile->setIsOpen(true);					// 오픈리스트인가?
				_vTotalList.push_back(_enemyTile);				// 토탈리스트에 넣기
				continue;
			}

			//그 외 타일
			astarTile* node = new astarTile;
			node->init(j, i);
			node->setIsOpen(true);
			node->setAttribute("null");

			// 그 외 타일에서 장애물이면 색상 변경 및 통과 불가능하게 변경 - 카메라 적용되면 다시 수정
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
		//// ############ 벡터 안터지게 #####################
		if (startY + i < 0)				continue;
		if (startY + i >= TILEY)		continue;
		for (int j = 0; j < 3; ++j)
		{
			// ############ 신항로 개척 방지 #####################
			if (startX + j < 0)			continue;
			if (startX + j >= TILEX)	continue;

			int checkIndex = (startY * TILEX) + startX + j + (i * TILEX);
			astarTile* node = _vTotalList[checkIndex];

			//예외처리!
			if (!node->getIsOpen()) continue;
			if (node->getAttribute() == "player") continue;
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

void garDen::pathFinder(astarTile * currentTile)
{//경로비용을 매우 쉽게 하기 위해서 임의의 경로비용을 둠
	float tempTotalCost = 5000;
	astarTile* tempTile = nullptr;

	bool changed = false;

	//갈수 있는 길을 담은 벡터 내부에서 가장 빠른 경로를 뽑아야한다
	for (int i = 0; i < addOpenList(currentTile).size(); ++i)
	{ // 여기서 주변방향 넣음과 동시에 리턴으로 총오픈리스트를 가져오는거
		_vOpenList[i]->setCostToGoal( // H를 계산(끝점과의 수직거리)
			(abs(_enemyTile->getIdx() - _vOpenList[i]->getIdx()) +
				abs(_enemyTile->getIdy() - _vOpenList[i]->getIdy())) * 10);

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
	if (tempTile->getAttribute() == "enemy")
	{
		_enemyTile->setAttribute("");
		//_enemyTile->setColor(RGB(250, 150, 0));
		while (_currentTile->getParentNode() != NULL)
		{
			++_numCount;
			_currentTile->setNumber(_numCount); // 숫자를 지정해줄수 있음
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

#include "stdafx.h"
#include "bossStage.h"
#include "Player.h"
HRESULT bossStage::init()
{
	//카메라 초기 세팅하는 부분
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);
	_playerTile = new astarTile;
	_enemyTile = new astarTile;
	dead = new DeadManager;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		_objTile[i] = new astarTile;
	}
	_boss = new boss;
	load();
	_player->init();
	_player->setDirec(CHRDIREC_UP);
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	CAMERAMANAGER->setCamera(camera);
	alpha = 1;
	_Stop = false;
	objectLocation();
	dead->setPlayerAddress(_player);
	clock = false;
	return S_OK;
}

void bossStage::release()
{
}

void bossStage::update()
{
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	//cout<<
	CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));
	if (!_Stop)
	{
		_boss->update();
		_player->update();

		if (_isBossAppeal)
		{
			playerLocation();
			//enemyLocation();
			// 셋타일 (처음에 변수가 쓰레기값이므로 여기서 한번만 적용시킴)
			if (!_setTile)
			{
				enemyLocation();
				setAstarTile();
				_setTile = true;
			}

			_follow_count++;
			if (_follow_count >= 10)
			{
				resetEverything();
				_currentTile = _playerTile;
				while (_numCount == 0 && !_stop) pathFinder(_currentTile);
				if (Math::GetDistance(_playerTile->getIdx(), _playerTile->getIdy(), _enemyTile->getIdx(), _enemyTile->getIdy()) < 1.5f)
				{
					resetEverything();
					this->release();
					dead->setDead(DEAD_BOSS);
					dead->update();
		
					_stop = true;
					return;
				}
				_follow_count = 0;
			}
		}
	}

	tileCollision();
	activeTrigger();
	activeCorr();
}

void bossStage::render()
{

		_backGround->SetAlpha(alpha);
		CAMERAMANAGER->render(_backGround, Vector2(_backGround->GetSize().x / 2 + 480, _backGround->GetSize().y / 2));
		for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEX; j++)
			{
				if (KEYMANAGER->isToggleKey(VK_TAB))
				{
					CAMERAMANAGER->renderRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::White, 1, 1);
					if (_tiles[i*TILEX + j].isCollider)
					{
						CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Red, 0.4);
					}
					if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aqua, 0.5);
				}

			}
		}
		_player->render();
		if (_isBossAppeal&&!dead->getIsDead())_boss->render();
		for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEX; j++)
			{
				if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;
				//IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->SetAlpha(0.5);
				IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->SetAlpha(alpha);
				CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
					Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top));
			}
		}
		dead->render();

}

void bossStage::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/ElenRoom.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	_backGround = IMAGEMANAGER->FindImage(_tiles->backGroundName);
	cout << _tiles->camera.x;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute == PLAYER)
		{
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}
		if (_tiles[i].attribute == ENEMY)
		{
			bossLocX = i % TILEX;
			bossLocY = i / TILEX;
		}
		if (_tiles[i].attribute == OBJ)
		{
		
		}
	}

	CloseHandle(file);
}

void bossStage::tileCollision()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (IntersectRectToRect(&_player->getPlayerFrc(), &_tiles[i*TILEX + j].rc) && _tiles[i*TILEX + j].isCollider)
			{
				switch (_player->getPdirec())
				{
				case CHRDIREC_DOWN:
					_player->setPLocaY(_tiles[i*TILEX + j].rc.top - TILESIZE/3*2 );
					break;
				case CHRDIREC_LEFT:
					_player->setPLocaX(_tiles[i*TILEX + j].rc.right +4);
					break;
				case CHRDIREC_RIGHT:
					_player->setPLocaX(_tiles[i*TILEX + j].rc.left-TILESIZE/3*2);
					break;
				case CHRDIREC_UP:
					_player->setPLocaY(_tiles[i*TILEX + j].rc.bottom+4);
					break;
				}
			}
		}
	}
}

void bossStage::activeTrigger()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (IntersectRectToRect(&_player->getSearchRc(), &_tiles[i*TILEX + j].rc) && _tiles[i*TILEX + j].terrain == TR_TRIGGER&&!_isBossAppeal)
			{
				if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
				{
					_boss->init(bossLocX, bossLocY);
					_isBossAppeal = true;
				}
			}
		}
	}

}

void bossStage::activeCorr()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (IntersectRectToRect(&_player->getPlayerFrc(), &_tiles[i*TILEX + j].rc) && _tiles[i*TILEX + j].obj == OBJ_CORELATION)
			{
				_Stop = true;
				alpha -= 0.01;
				_player->setAlpha(alpha);
				_boss->setalpha(alpha);
				if (alpha <= 0)
				{
					SCENEMANAGER->changeScene("Boss2");
					_boss->setalpha(1);
					_player->setAlpha(1);
				}
			}
		}
	}
}

void bossStage::playerLocation()
{
	_playerTile->init(_player->getPlayerLocX()/TILESIZE,
		_player->getPlayerLocY()/TILESIZE);
	_playerTile->setAttribute("player");	// astar타일로 플레이어 적용
}

void bossStage::enemyLocation()
{
	_enemyTile->init(bossLocX,bossLocY);			// astar타일로 지속적인 적용
	_enemyTile->setAttribute("enemy");			// astar타일로 에너미 적용
}

void bossStage::objectLocation()
{
	for (int i = 0; i < (TILEX * TILEY); i++)
	{
		if (_tiles[i].isCollider == true)
		{
			_objTile[MaxIndex]->init(_tiles[i].rc.left / TILESIZE, _tiles[i].rc.top / TILESIZE);
			MaxIndex++;
		}
	}
	cout << MaxIndex<<endl;
	for (int i = 0; i < MaxIndex; i++)
	{
		_objTile[i]->setAttribute("wall");
	}
}

void bossStage::setAstarTile()
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
				_enemyTile->setColor(D2D1::ColorF::Red);		// 에너미 타일 색상 - 사실상 구분용 의미없을듯
				_enemyTile->setIsOpen(true);					// 오픈리스트인가?
				_vTotalList.push_back(_enemyTile);				// 토탈리스트에 넣기
				continue;
			}

			//그 외 타일
			astarTile* node = new astarTile;
			node->init(j, i);
			node->setColor(D2D1::ColorF::White);
			node->setIsOpen(true);
			node->setAttribute("");

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

void bossStage::resetEverything()
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

vector<astarTile*> bossStage::addOpenList(astarTile * currentTile)
{
	int startX = currentTile->getIdx() - 1;
	int startY = currentTile->getIdy() - 1;

	for (int i = 2; i >=0; --i)
	//for (int i = 0; i < 3; ++i)
	{
		//// ############ 벡터 안터지게 #####################
		if (startY + i < 0)				continue;
		if (startY + i >= TILEY)		continue;
		for (int j =2; j >=0; --j)
		//for (int j = 0; j<3; ++j)
		{
			// ############ 신항로 개척 방지 #####################
			if (startX + j < 0)			continue;
			if (startX + j >= TILEX)	continue;

			int checkIndex = (startY * TILEX) + startX + j + (i * TILEX);
			// startX = 13, startY = 15	-> 195
			// 1. 600 + 13 + 0 + (0 * 40) = 613 2. 600 + 13 + 1 + (0 * 40) = 614 3. 600 + 13 + 2 + (0 * 40) = 615
			// 4. 600 + 13 + 0 + (1 * 40) = 653 5. 600 + 13 + 1 + (1 * 40) = 654 6. 600 + 13 + 2 + (1 * 40) = 655
			// 7. 600 + 13 + 0 + (2 * 40) = 693 8. 600 + 13 + 1 + (2 * 40) = 693 9. 600 + 13 + 2 + (2 * 40) = 695
			if (i == 0 && j == 0) continue; // 왼쪽 위
			if (i == 2 && j == 0) continue; // 왼쪽 아래
			if (i == 2 && j == 2) continue; // 오른쪽 아래
			if (i == 0 && j == 2) continue; // 오른쪽 위
			//cout << checkIndex << endl;
			//if (checkIndex == (startX * startY - TILEY - 1)) continue;
			//if (checkIndex == (startX * startY - TILEY + 1)) continue;
			//if (checkIndex == (startX * startY + TILEY - 1)) continue;
			//if (checkIndex == (startX * startY + TILEY + 1)) continue;
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
		//cout << endl << endl;
	}
	return _vOpenList;
}

void bossStage::pathFinder(astarTile * currentTile)
{
	//경로비용을 매우 쉽게 하기 위해서 임의의 경로비용을 둠
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
			center1.y == center2.y) ? 14 : 10);
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
		_enemyTile->setColor(D2D1::ColorF::White);
		while (_currentTile->getParentNode() != NULL)
		{
			++_numCount;
			_currentTile->setNumber(_numCount); // 숫자를 지정해줄수 있음
			if (_numCount == 1)
			{
				bossLocX = _currentTile->getIdx();
				bossLocY = _currentTile->getIdy();
				_vTotalList[bossLocY*TILEX + bossLocX]->setAttribute("enemy");
				_vTotalList[bossLocY*TILEX + bossLocX]->setColor(D2D1::ColorF::Red);
				_enemyTile = _vTotalList[bossLocY*TILEX + bossLocX];
				//_boss->setRect(FloatRect(_enemyTile->getRect()));
				_boss->setX(_enemyTile->getRect().left);			// 에너미 움직임 X
				_boss->setY(_enemyTile->getRect().top);				// 에너미 움직임 Y
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

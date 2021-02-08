#include "stdafx.h"
#include "prison_5f.h"
#include "Player.h"

HRESULT prison_5f::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	SOUNDMANAGER->play("감옥");
	_real_location1 = PRISON;
	_player->setDirec(_chrdirection);

	//타일 불러오기
	load(_location); 

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	fifthFloorStage::init();

	getMemory();
	_skulX = _tiles[SUMMONSKUL].rc.left / TILESIZE;
	_skulY = _tiles[SUMMONSKUL].rc.top / TILESIZE;
	_playerTile = new astarTile;
	_enemyTile = new astarTile;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		_objTile[i] = new astarTile;
	}
	objectLocation();
	_sound = false;
	return S_OK;
}

void prison_5f::release()
{
	SOUNDMANAGER->stop("감옥");
	SOUNDMANAGER->stop("적이쫒을때");
	_sound = false;
}

void prison_5f::update()
{
	if (!_isStopToRead)
	{
		fifthFloorStage::update();
		setFrameIndex();

		//카메라 관련 업데이트
		cameraUpdate();
		setTrigger();
		
		if (_isSummon)
		{
			playerLocation();
			// 셋타일 (처음에 변수가 쓰레기값이므로 여기서 한번만 적용시킴)
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
					SOUNDMANAGER->play("해골데스");
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
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// 클릭행동 트리거
		{
			_isStopToRead = TEXTMANAGER->setNextScript(true);
			_sound = false;

			if (_sound_item && !_isStopToRead)
			{
				SOUNDMANAGER->play("getItem");
				_sound_item = false;
			}
		}
	}
}

void prison_5f::render()
{	
	//배경 랜더
	IMAGEMANAGER->FindImage("배경65")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경65"),
		Vector2(IMAGEMANAGER->FindImage("배경65")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("배경65")->GetSize().y / 2));

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

		//초기 위치를 잡아준다.
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

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// 클릭행동 트리거
	{
		_isStopToRead = TEXTMANAGER->setNextScript(true);

		if (IntersectRectToRect(&_tiles[R_FLOWER_1].rc, &_player->getSearchRc()))
		{
			if (!_sound)
				SOUNDMANAGER->play("여자비웃음");
			_sound = true;
			STAGEMEMORYMANAGER->setIsRedFlower1(true);
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_flower_1.txt");
			_isStopToRead = true;
		}
		if (IntersectRectToRect(&_tiles[R_FLOWER_2].rc, &_player->getSearchRc()))
		{
			if (!_sound)
				SOUNDMANAGER->play("여자비웃음");
			_sound = true;
			STAGEMEMORYMANAGER->setIsRedFlower2(true);
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_flower_2.txt");
			_isStopToRead = true;
		}
		if (IntersectRectToRect(&_tiles[R_FLOWER_3].rc, &_player->getSearchRc()))
		{
			if (!_sound)
				SOUNDMANAGER->play("여자비웃음");
			_sound = true;
			STAGEMEMORYMANAGER->setIsRedFlower3(true);
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_flower_3.txt");
			_isStopToRead = true;
		}
		if (IntersectRectToRect(&_tiles[BOOK].rc, &_player->getSearchRc()))
		{
			if (!_sound)
				SOUNDMANAGER->play("openBook");
			_sound = true;
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_book.txt");
			_isStopToRead = true;
		}
		if (IntersectRectToRect(&_tiles[PAPER].rc, &_player->getSearchRc()))
		{
			cout << "힌트!" << endl;
		}
		if (IntersectRectToRect(&_tiles[DOOR_2].rc, &_player->getSearchRc()))
		{
			if (!_sound)
				SOUNDMANAGER->play("철문");
			_sound = true;
			switch (_ido)
			{
			case prison_5f::FIRST:
				_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_irondoor_1.txt");
				_isStopToRead = true;
				_ido = SECOND;
				break;
			case prison_5f::SECOND:
				_sound_item = true;
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
		if (IntersectRectToRect(&_tiles[CAGE].rc, &_player->getSearchRc()) && STAGEMEMORYMANAGER->getIsKey())
		{
			_vFrameTile[4].isTrigger = true;
			if (!_sound)
				SOUNDMANAGER->play("새");
			_sound = true;
			_sound_item = true;
		}
		else if (IntersectRectToRect(&_tiles[CAGE].rc, &_player->getSearchRc()))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_cage.txt");
			_isStopToRead = true;
		}
	}

	if ((IntersectRectToRect(&_tiles[SKUL].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[SKUL - 1].rc, &_player->getPlayerFrc()))&&
		!_isSummon && !_setTile && STAGEMEMORYMANAGER->getIsLever())
	{
		if (!_sound)
		{
			SOUNDMANAGER->stop("감옥");
			SOUNDMANAGER->play("적이쫒을때");
			SOUNDMANAGER->play("쾅");
		}
		_sound = true;
		_skul->init(_skulX, _skulY);
		_isSkulAppeal = true;
		_isSummon = true;
	}
	if (IntersectRectToRect(&_tiles[DOORTOGARDEN].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOGARDEN + TILEX].rc, &_player->getPlayerFrc()))
	{
		if (!_sound)
			SOUNDMANAGER->play("철문");
		_sound = true;
		_isChangeScene = true;
		sceneChange("garden_5f", CHRDIREC_LEFT, LOCATION_3);
	}
	if (IntersectRectToRect(&_tiles[DOORTOPRISONWELL].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISONWELL + 1].rc, &_player->getPlayerFrc()))
	{
		if (!_sound)
			SOUNDMANAGER->play("철문");
		_sound = true;
		_vFrameTile[0].isTrigger = true;
		_isChangeScene = true;
		sceneChange("prison_well_5f", CHRDIREC_UP, LOCATION_DEFAULT);
	}
	if (IntersectRectToRect(&_tiles[DOORTOPRISONSKUL].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISONSKUL + TILEX].rc, &_player->getPlayerFrc()))
	{
		if (!_sound)
			SOUNDMANAGER->play("철문");
		_sound = true;
		_isChangeScene = true;
		sceneChange("prison_skul_5f", CHRDIREC_RIGHT, LOCATION_DEFAULT);
	}
}

void prison_5f::getMemory()
{
	for (int k = 0; k < _vFrameTile.size(); k++)
	{
		if (!STAGEMEMORYMANAGER->getIsPotion()) continue;

		if (_vFrameTile[k].keyName == "새장")
		{
			//트리거가 이미 발동되었던 상태로 셋팅한다.
			_vFrameTile[k].isMaxframe = true;
		}
	}
}

void prison_5f::playerLocation()
{
	_playerTile->init(_player->getPlayerLocX() / TILESIZE,
		_player->getPlayerLocY() / TILESIZE);
	_playerTile->setAttribute("player");	// astar타일로 플레이어 적용
}

void prison_5f::enemyLocation()
{
	_enemyTile->init(_skulX, _skulY);			// astar타일로 지속적인 적용
	_enemyTile->setAttribute("enemy");			// astar타일로 에너미 적용
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
	for (int i = 0; i < MaxIndex; i++)
	{
		_objTile[i]->setAttribute("wall");
	}
}

void prison_5f::setAstarTile()
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
		//// ############ 벡터 안터지게 #####################
		if (startY + i < 0)				continue;
		if (startY + i >= TILEY)		continue;
		for (int j = 2; j >= 0; --j)
			//for (int j = 0; j<3; ++j)
		{
			// ############ 신항로 개척 방지 #####################
			if (startX + j < 0)			continue;
			if (startX + j >= TILEX)	continue;

			int checkIndex = (startY * TILEX) + startX + j + (i * TILEX);
			if (i == 0 && j == 0) continue; // 왼쪽 위
			if (i == 2 && j == 0) continue; // 왼쪽 아래
			if (i == 2 && j == 2) continue; // 오른쪽 아래
			if (i == 0 && j == 2) continue; // 오른쪽 위
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

void prison_5f::pathFinder(astarTile * currentTile)
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

		_vOpenList[i]->setCostFromStart(10);
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
				_skulX = _currentTile->getIdx();
				_skulY = _currentTile->getIdy();
				_vTotalList[_skulY*TILEX + _skulX]->setAttribute("enemy");
				_vTotalList[_skulY*TILEX + _skulX]->setColor(D2D1::ColorF::Red);
				_enemyTile = _vTotalList[_skulY*TILEX + _skulX];

				_skul->setX(_enemyTile->getRect().left);			// 에너미 움직임 X
				_skul->setY(_enemyTile->getRect().top);				// 에너미 움직임 Y
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

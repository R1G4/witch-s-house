#include "stdafx.h"
#include "firstFloorStage.h"
#include "Player.h"

firstFloorStage::firstFloorStage()
{
}

firstFloorStage::~firstFloorStage()
{
}

HRESULT firstFloorStage::init()
{
	_player->init();
	_player->setAlpha(1.f);

	//ī�޶� ���� �� �𸣰ڴ� �� �ʱ�ȭ ����..
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);
	CAMERAMANAGER->setCamera(camera);

	getFrameTile();
	_sceneAlpha = 0.05;
	_delay = 0;
	_light = 1.f;
	return S_OK;
}

void firstFloorStage::release()
{
	//���ʹ̰� ������ �� �޸� ���� �� ����
	if (_bear)
	{
		SAFE_RELEASE(_bear);
		SAFE_DELETE(_bear);
	}

	//���� ����� ������ �� �޸� ���� �� ����
	if (_dead)
	{
		SAFE_RELEASE(_dead);
		SAFE_DELETE(_dead);
	}

	//A* ���� ����Ʈ ����
	_vTotalList.clear();
	_vOpenList.clear();
	_vCloseList.clear();
	_vFrameTile.clear();
	if (_playerTile)_playerTile->release();					// �÷��̾� Ÿ��
	if (_enemyTile)_enemyTile->release();					// ���ʹ� Ÿ��
	if (_objTile[TILEX*TILEY])_objTile[TILEX*TILEY]->release();		// ������Ʈ Ÿ��
	if (_currentTile)_currentTile->release();

	//dialog ���� ����
	_vScript.clear();
	TEXTMANAGER->clearScript();

	//Z ���� �޸� ����
	ZORDER->release();
}

void firstFloorStage::update()
{

	//�÷��̾� ����â�� ���Ȱų� Ȥ�� ������� �����ϸ� ������� ����� ���
	if (_isForm || (_dead && _dead->getIsDead()))
	{
		//������Ʈ�� �����ϰ� �����Ӹ� ������
		_player->framePlay();
	}
	else
		_player->update();
	
}

void firstFloorStage::enemyUpdate()
{
	//���ʹ� ������Ʈ
	_bear->update();

	//���ʹ̰� �����Ѵٸ� A* �����Ѵ�. 
	if (_bear)
	{
		playerLocation();

		if (!_setTile)
		{
			enemyLocation();
			setAstarTile();
			_setTile = true;
		}

		_follow_count++;
		if (_follow_count >= 13)
		{
			resetEverything();
			_currentTile = _playerTile;
			while (_numCount <= 0 && !_stop)
			{
				pathFinder(_currentTile);
			}
			if (Math::GetDistance(_playerTile->getIdx(), _playerTile->getIdy(), _enemyTile->getIdx(), _enemyTile->getIdy()) < 1.1f)
			{
				//���ʹ̰� �������� �ʴٸ� ��ȯ�Ѵ�.
				if (!_bear)	return;
				resetEverything();			

				//����� ����
				_dead->setDead(DEAD_BEAR);
				_dead->update();

				//���ʹ� �޸� ���� ������
				SAFE_RELEASE(_bear);
				SAFE_DELETE(_bear);

				_stop = true;
				return;
			}
			_follow_count = 0;
		}
	}
}

void firstFloorStage::cameraUpdate()
{
	CAMERAMANAGER->setWorldMouse(_ptMouse);
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));
}

void firstFloorStage::render()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			//if (!_tiles[i*TILEX + j].isMapOn)continue;
			if (KEYMANAGER->isToggleKey(VK_TAB))
			{
				CAMERAMANAGER->renderRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::White, 1, 1);
				if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aqua, 0.5);
				if (_tiles[i*TILEX + j].isCollider)CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Red, 0.5);
			}
		}
	}
	//Z-order ���Ϳ� �����͸� �־��ִ� ����
		//�÷��̾�� ���ʹ��� ��� �� ��ü�� �����Լ��� ȣ���� ���̹Ƿ� z-order�񱳸� ���� y���� Ÿ�԰��� �־��ָ� ���
	ZORDER->insert(_player->getPlayerFrc().left, _player->getPlayerFrc().top, ZPLAYER);
	if(_bear)	ZORDER->insert(_bear->getRect().left, _bear->getRect().top, ZENEMY);
//	if (_isBossAppeal)ZORDER->insert(_bear->getRect().left, _bear->getRect().top, ZENEMY);
	//������Ʈ�� �־��ִ� ����
	//������Ʈ�� ��� ������ ������ϹǷ� �̹����� �־��ְų� Ű���� �־��ִ°� ����
	//Ư�� Ʈ���ŷ� ����ó���� �ؾ��Ѵٸ�, �̹����� �ٷ� �ֱ⺸�ٴ� Ű���� �־��ִ� ����� ����ϴ°� ���ƺ���
	//������Ʈ�� ���� Ÿ�ϱ��� �ְԵǸ� ���α׷��� �������� �����ϱ����
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].obj != OBJ_NONE)
		{
			if (_tiles[i].keyName == "obj5" || _tiles[i].keyName == "obj9")continue;
			if (!STAGEMEMORYMANAGER->getIsBearComing2() && (_tiles[i].keyName == "obj62" || _tiles[i].keyName == "obj63"))continue;
			ZORDER->insert(_tiles[i].rc.left, _tiles[i].rc.bottom, IMAGEMANAGER->FindImage(_tiles[i].keyName), ZOBJECT);
		}
	}
	//������ ������Ʈ���� �־��ִ� ����
	//�����̰� ���� �����������Ŵ����� Ȱ���ϰ� �� ��� �ٲ���� ���ɼ� ����
	for (int i = 0; i < _vFrameTile.size(); i++)
	{
		ZORDER->insert(_vFrameTile[i].rc.left, _vFrameTile[i].rc.top, _vFrameTile[i].keyName, ZFRAMEOBJ);
	}
	for (int i = 0; i < ZORDER->getZorder().size(); i++)
	{
		cout << ZORDER->getZorder()[i].y << " " << ZORDER->getZorder()[i].type << " / ";
	}
	//���ĵ� ������� ����
	//�� �ε����� Ÿ���� Ȯ���Ͽ� Ÿ�Կ� ���� �̹����� �����ϵ��� ������
	//�� �κп��� FrameInfoManager�� �� ���ư��� ������ �𸣰��� �����ʿ�
	for (int i = 0; i < ZORDER->getZorder().size(); i++)
	{
		if (ZORDER->getZorder()[i].type == ZPLAYER)_player->render();
		if (ZORDER->getZorder()[i].type == ZENEMY && _bear)
			_bear->render();
		if (ZORDER->getZorder()[i].type == ZOBJECT)
		{
			if (!STAGEMEMORYMANAGER->getIsBearComing2() && (ZORDER->getZorder()[i].keyName == "obj62" || ZORDER->getZorder()[i].keyName == "obj63"))continue;
			CAMERAMANAGER->render(ZORDER->getZorder()[i].img,
				Vector2(ZORDER->getZorder()[i].x + TILESIZE / 2, ZORDER->getZorder()[i].y - ZORDER->getZorder()[i].img->GetSize().y / 2));
		}
		if (ZORDER->getZorder()[i].type == ZFRAMEOBJ)
		{
			for (int j = 0; j < _vFrameTile.size(); j++)
			{
				if (_vFrameTile[j].kinds == PLAYER) continue;
				if (_vFrameTile[j].kinds == ENEMY)	continue;
				if (_vFrameTile[j].keyName == "�չٴ�" && !_isBlood) continue;
				if (ZORDER->getZorder()[i].keyName == _vFrameTile[j].keyName)
					CAMERAMANAGER->FrameRender
					(
						_vFrameTile[j].img,
						Vector2((_vFrameTile[j].rc.left + _vFrameTile[j].rc.right) / 2, _vFrameTile[j].rc.bottom - _vFrameTile[j].img->GetSize().y / 2),
						_vFrameTile[j].frameX, _vFrameTile[j].frameY
					);
			}
		}
	}
	//zorder ���͸� �ʱ�ȭ���� ���ϸ� ������
	ZORDER->release();
	if(_dead) _dead->render();
	//������ Ÿ�� ������Ʈ�̹������� �ְԵǸ� ���α׷��� ���ſ����� ���� ��
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].keyName == "obj5" || _tiles[i].keyName == "obj9")
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i].keyName),
				Vector2(_tiles[i].rc.left + TILESIZE / 2, _tiles[i].rc.bottom - IMAGEMANAGER->FindImage(_tiles[i].keyName)->GetSize().y / 2));
	}
	IMAGEMANAGER->FindImage("Back2")->SetAlpha(_light);
	IMAGEMANAGER->FindImage("Back2")->SetSize(Vector2(1920, 1280));
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("Back2"), Vector2(_player->getPlayerLocX(), _player->getPlayerLocY()));

	if (_vScript.size() > 0) TEXTMANAGER->renderText();

	//���� ����ǰ� �ִٸ�
	if (_isForm)	FormRender();
}

void firstFloorStage::getFrameTile()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			//�̹��� ������ �������� �ʴ´ٸ� ���ʿ��� �ݺ����� �ǳʶڴ�.
			if (!FRAMEINFOMANAGER->GetSize())
				return;

			if (!FRAMEINFOMANAGER->KeyCheck(_tiles[i*TILEX + j].keyName))
				continue;

			//��Ʈ ����
			FloatRect rc;
			rc = RectMakePivot(Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2), Vector2(TILESIZE, TILESIZE), Pivot::Center);

			FRAMEATTRIBUTE tempKinds = FRAMEINFOMANAGER->GetAttribute(_tiles[i*TILEX + j].keyName);

			tagFrameTile temp;
			temp.rc = rc;
			temp.kinds = tempKinds;
			temp.keyName = _tiles[i*TILEX + j].keyName;
			temp.indexX = i;
			temp.indexY = j;
			temp.frameX = 0;
			temp.frameY = 0;
			temp.img = FRAMEINFOMANAGER->FindImage(_tiles[i*TILEX + j].keyName);

			_vFrameTile.push_back(temp);
		}
	}
}

void firstFloorStage::setFrameIndex()
{
	//����) Ÿ�Ͽ� ��ġ�� �������� ���� ���
	if (_vFrameTile.size() <= 0)
		return;

	for (int i = 0; i < _vFrameTile.size(); i++)
	{
		if (_vFrameTile[i].isMaxframe)
		{
			_vFrameTile[i].frameX = FRAMEINFOMANAGER->FindImage(_vFrameTile[i].keyName)->GetMaxFrameX() - 1;
			_vFrameTile[i].frameY = FRAMEINFOMANAGER->FindImage(_vFrameTile[i].keyName)->GetMaxFrameY() - 1;
			continue;
		}

		Vector2 temp;
		temp = FRAMEINFOMANAGER->FrameOperation(_vFrameTile[i].keyName, Vector2(_vFrameTile[i].frameX, _vFrameTile[i].frameY), _vFrameTile[i].isTrigger);
		_vFrameTile[i].frameX = temp.x;
		_vFrameTile[i].frameY = temp.y;
	}
}

void firstFloorStage::tileCollision(int i, int j)
{
	if (!_tiles[i*TILEX + j].isCollider) return;

	switch (_player->getPdirec())
	{
	case CHRDIREC_DOWN:
		_player->setPLocaY(_tiles[i*TILEX + j].rc.top - TILESIZE / 4 * 3);
		break;
	case CHRDIREC_LEFT:
		_player->setPLocaX(_tiles[i*TILEX + j].rc.right + 4);
		break;
	case CHRDIREC_RIGHT:
		_player->setPLocaX(_tiles[i*TILEX + j].rc.left - TILESIZE / 4 * 3);
		break;
	case CHRDIREC_UP:
		_player->setPLocaY(_tiles[i*TILEX + j].rc.bottom + 4);
		break;
	}
}

void firstFloorStage::sceneChange(string name, CHRDIRECTION _chrdirection, LOCATION _location)
{
	_sceneAlpha -= 0.008f;
	if (_sceneAlpha <= 0.65f)
		_sceneAlpha -= 0.021f;

	_player->setAlpha(_sceneAlpha);

	if (_sceneAlpha <= 0.f)	SCENEMANAGER->changeScene(name, _chrdirection, _location);
}

void firstFloorStage::setAlpha()
{
	if (_sceneAlpha < 1.f)
	{
		if (_sceneAlpha <= 0.65f)
			_sceneAlpha += 0.019f;
		else
			_sceneAlpha += 0.008f;
		if (_sceneAlpha >= 0.9f)
			_sceneAlpha = 1;

		_player->setAlpha(_sceneAlpha);
	}
}

bool firstFloorStage::SelectionForm(wstring leftText, wstring rightText)
{
	if (!_isForm && KEYMANAGER->isOnceKeyDown(VK_SPACE))
		_isForm = true;

	if (!_isForm) return false;

	FormInfo[LEFT] = leftText;
	FormInfo[RIGHT] = rightText;

	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		_fromSelected = (FROM_DIRECTION)(_fromSelected - 1 > -1 ? _fromSelected - 1 : _fromSelected);
	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		_fromSelected = (FROM_DIRECTION)(_fromSelected + 1 < 2 ? _fromSelected + 1 : _fromSelected);

	//���õǴ� ȿ���� �����ش�.(��Ʈ)
	switch (_fromSelected)
	{
	case firstFloorStage::LEFT:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2 - 50), Vector2(330, 88), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE) * 0x20)
		{
			_isForm = false;
			return true;
		}
		break;
	case firstFloorStage::RIGHT:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2 - 50), Vector2(330, 88), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE) * 0x20)
		{
			_isForm = false;
			return true;
		}
		break;
	}

	rcAlphaChange();

	return false;
}

void firstFloorStage::FormRender()
{
	//if (!_isForm) return;
	IMAGEMANAGER->FindImage("bar")->SetScale(1.1f);
	IMAGEMANAGER->FindImage("bar")->Render(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2 - 50));	//����
	IMAGEMANAGER->FindImage("bar")->SetScale(1.1f);
	IMAGEMANAGER->FindImage("bar")->Render(Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2 - 50)); // ������

	//�ؽ�Ʈ �ڽ��� ���� ���ڰ� ��� �˾Ƽ� ��������
	D2DINS->GetInstance()->RenderTextField(263, WINSIZEY / 2 - 150, FormInfo[LEFT], RGB(255, 255, 255), 27, 280, 200, 0.85f, DWRITE_TEXT_ALIGNMENT_LEADING);
	D2DINS->GetInstance()->RenderTextField(748, WINSIZEY / 2 - 150, FormInfo[RIGHT], RGB(255, 255, 255), 27, 280, 200, 0.85f, DWRITE_TEXT_ALIGNMENT_LEADING);

	//��׶��� �÷� ��Ʈ
	D2DINS->FillRectangle
	(
		_rcSelected,
		D2D1::ColorF::Enum::WhiteSmoke,
		_rcAlpha / 5.5f
	);
	//�׵θ� �÷� ��Ʈ
	D2DINS->DrawRectangle
	(
		_rcSelected,
		D2D1::ColorF::Enum::White,
		_rcAlpha,
		1.0f
	);
}

void firstFloorStage::rcAlphaChange()
{
	if (_rcAlpha >= 0.9f)
		_rcAlphaChange = -0.03f;

	if (_rcAlpha <= 0.35f)
		_rcAlphaChange = 0.03f;

	_rcAlpha += _rcAlphaChange;
}

void firstFloorStage::playerLocation()
{
	int tem = _player->getPlayerLocY();
	int tem2 = _player->getPlayerLocX();
	_playerTile->init((_player->getPlayerLocX() + TILESIZE / 2) / TILESIZE,
		(_player->getPlayerLocY() + TILESIZE / 2) / TILESIZE);
	_playerTile->setAttribute("player");	// astarŸ�Ϸ� �÷��̾� ����
}

void firstFloorStage::enemyLocation()
{
	_enemyTile->init(bossLocX, bossLocY);			// astarŸ�Ϸ� �������� ����
	_enemyTile->setAttribute("enemy");			// astarŸ�Ϸ� ���ʹ� ����
}

void firstFloorStage::objectLocation()
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

void firstFloorStage::setAstarTile()
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

void firstFloorStage::resetEverything()
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

vector<astarTile*> firstFloorStage::addOpenList(astarTile * currentTile)
{
	int startX = currentTile->getIdx() - 1;
	int startY = currentTile->getIdy() - 1;

	//for (int i = 2; i >= 0; --i)
		for (int i = 0; i < 3; ++i)
	{
		//// ############ ���� �������� #####################
		if (startY + i < 0)				continue;
		if (startY + i >= TILEY)		continue;
		//for (int j = 2; j >= 0; --j)
			for (int j = 0; j<3; ++j)
		{
			// ############ ���׷� ��ô ���� #####################
			if (startX + j < 0)			continue;
			if (startX + j >= TILEX)	continue;

			int checkIndex = (startY * TILEX) + startX + j + (i * TILEX);
			if (i == 0 && j == 0) continue; // ���� ��
			if (i == 2 && j == 0) continue; // ���� �Ʒ�
			if (i == 2 && j == 2) continue; // ������ �Ʒ�
			if (i == 0 && j == 2) continue; // ������ ��
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

void firstFloorStage::pathFinder(astarTile * currentTile)
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
				bossLocX = _currentTile->getIdx();
				bossLocY = _currentTile->getIdy();
				_vTotalList[bossLocY*TILEX + bossLocX]->setAttribute("enemy");
				_vTotalList[bossLocY*TILEX + bossLocX]->setColor(D2D1::ColorF::Red);
				_enemyTile = _vTotalList[bossLocY*TILEX + bossLocX];
				//_bear->setRect(FloatRect(_enemyTile->getRect()));
				_bear->setX(_enemyTile->getRect().left);			// ���ʹ� ������ X
				_bear->setY(_enemyTile->getRect().top);				// ���ʹ� ������ Y
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
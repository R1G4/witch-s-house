#include "stdafx.h"
#include "diningRoom_5f.h"
#include "Player.h"

HRESULT diningRoom_5f::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_player->setDirec(_chrdirection);

	//Ÿ�� �ҷ�����
	load(_location);

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	fifthFloorStage::init();

	getMemory();
	_isClock = false;
	return S_OK;
}

void diningRoom_5f::release()
{
}

void diningRoom_5f::update()
{
	if (!_isClick && !_isStopToRead)
	{
		fifthFloorStage::update();

		setFrameIndex();

		//ī�޶� ���� ������Ʈ
		cameraUpdate();
		setTrigger();
	}
	setChoiceScene(); 
	
	if (_isStopToRead)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// Ŭ���ൿ Ʈ����
			_isStopToRead = TEXTMANAGER->setNextScript(true);
		
		if (!_isStopToRead && _isClock)
		{
			_co = SECOND;
			_isClick = true;
		}
	}
	//cout << "x : " << (int)(_player->getPlayerLocX()) / TILESIZE << " y : " << (int)(_player->getPlayerLocY()) / TILESIZE << endl;
}

void diningRoom_5f::render()
{	
	//��� ����
	IMAGEMANAGER->FindImage("���62")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("���62"),
		Vector2(IMAGEMANAGER->FindImage("���62")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("���62")->GetSize().y / 2));

	fifthFloorStage::render();

	if (_isStopToRead)
		TEXTMANAGER->renderText();

	if (_isClick)
	{
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 250, camera.y));	//����
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x + 250, camera.y));	//������

		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 300, 345, L"������.", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 117, 345, L"�ƹ��͵� ���� �ʴ´�.", RGB(255, 255, 255), 0.85f, 27);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
	}
}

void diningRoom_5f::Collision()
{
}

void diningRoom_5f::load(LOCATION _location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/5f_diningRoom.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute != PLAYER) continue;

		//�ʱ� ��ġ�� ����ش�.
		switch (_location)
		{
		case LOCATION_DEFAULT: default:
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}
	}
	CloseHandle(file);
}

void diningRoom_5f::setTrigger()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// Ŭ���ൿ Ʈ����
	{
		if (IntersectRectToRect(&_tiles[Y_FLOWER_1].rc, &_player->getSearchRc()) &&
			STAGEMEMORYMANAGER->getIsFlowerDead())
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_flower_1_1.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		else if (IntersectRectToRect(&_tiles[Y_FLOWER_1].rc, &_player->getSearchRc()))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_flower_1.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		if (IntersectRectToRect(&_tiles[Y_FLOWER_2].rc, &_player->getSearchRc()) &&
			STAGEMEMORYMANAGER->getIsFlowerDead())
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_flower_2_1.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		else if (IntersectRectToRect(&_tiles[Y_FLOWER_2].rc, &_player->getSearchRc()))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_flower_2.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		if (IntersectRectToRect(&_tiles[Y_FLOWER_3].rc, &_player->getSearchRc()) &&
			STAGEMEMORYMANAGER->getIsFlowerDead() &&
			STAGEMEMORYMANAGER->getIsGetSkul3())
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_flower_2_1.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		else if (IntersectRectToRect(&_tiles[Y_FLOWER_3].rc, &_player->getSearchRc()) &&
			STAGEMEMORYMANAGER->getIsFlowerDead())
		{
			// �Ͼ�� ���̰� ������ �հ� ����
			STAGEMEMORYMANAGER->setIsGetSkul3(true);
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_flower_3_1.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		else if (IntersectRectToRect(&_tiles[Y_FLOWER_3].rc, &_player->getSearchRc()))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_flower_3.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		if (IntersectRectToRect(&_tiles[Y_FLOWER_4].rc, &_player->getSearchRc()) &&
			STAGEMEMORYMANAGER->getIsFlowerDead())
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_flower_4_1.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		else if (IntersectRectToRect(&_tiles[Y_FLOWER_4].rc, &_player->getSearchRc()))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_flower_4.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		if (IntersectRectToRect(&_tiles[CLOCK].rc, &_player->getSearchRc()) && _player->getPdirec() == CHRDIREC_UP)
		{
			_co = FIRST;
			switch (_co)
			{
			case FIRST:
				_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_clock.txt");
				_isStopToRead = true;
				_isClock = true;
				break;
			case SECOND: default:
				break;
			}
		}
		if (IntersectRectToRect(&_tiles[LIGHT].rc, &_player->getSearchRc()))
		{
			STAGEMEMORYMANAGER->setIsGetSkul1(true);
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_light.txt");
			_isStopToRead = true;
			_isClock = false;
		}
		if (IntersectRectToRect(&_tiles[DOORTODININGROOMITEM].rc, &_player->getPlayerFrc()))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_diningRoom_door.txt");
			STAGEMEMORYMANAGER->setIsKey(true);
			_isStopToRead = true;
			_isClock = false;
		}
	}

	if (IntersectRectToRect(&_tiles[DOORTOGARDEN].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOGARDEN + TILEX].rc, &_player->getPlayerFrc()))
	{
		_isChangeScene = true;
		sceneChange("garden_5f", CHRDIREC_RIGHT, LOCATION_1);
	}
	if (IntersectRectToRect(&_tiles[DROPFLOWER].rc, &_player->getPlayerFrc()))
	{
		STAGEMEMORYMANAGER->setIsFlowerpot(true);
		_vFrameTile[2].isTrigger = true;
	}

}

void diningRoom_5f::setChoiceScene()
{
	if (_isClick)	// �ð� ���� �̺�Ʈ
	{
		_correct_rc = RectMakePivot(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
		rcAlphaChange();
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
			_rc = RectMakePivot(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
			_rc = RectMakePivot(Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			_isClick = false;
			if (IntersectRectToRect(&_rc, &_correct_rc))
			{
				_dead->setDead(DEAD_CLOCK);
				_dead->setLocXY(_tiles[CLOCK].rc.left, _tiles[CLOCK].rc.top);
				_tiles[CLOCK].obj = OBJ_NONE;
				_player->~Player();
				_isDead = true;
			}
		}
	}
}

void diningRoom_5f::getMemory()
{
	for (int k = 0; k < _vFrameTile.size(); k++)
	{
		if (!STAGEMEMORYMANAGER->getIsFlowerpot()) continue;

		if (_vFrameTile[k].keyName == "�ɺ�������")
		{
			//Ʈ���Ű� �̹� �ߵ��Ǿ��� ���·� �����Ѵ�.
			_vFrameTile[k].isMaxframe = true;
		}
	}
}

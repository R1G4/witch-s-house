#include "stdafx.h"
#include "garden_5f.h"
#include "player.h"

HRESULT garden_5f::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_player->setDirec(_chrdirection);

	//Ÿ�� �ҷ�����
	load(_location);

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	fifthFloorStage::init();

	_x = WINSIZEX / 2 - 250;
	_y = WINSIZEY / 2;
	_rc = RectMakePivot(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
	_correct_rc = RectMakePivot(Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
	_co = FIRST;
	return S_OK;
}

void garden_5f::release()
{
}

void garden_5f::update()
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
	}
	//cout << "x : " << (int)(_player->getPlayerLocX()) / TILESIZE << " y : " << (int)(_player->getPlayerLocY()) / TILESIZE << endl;
}

void garden_5f::render()
{	
	//��� ����
	IMAGEMANAGER->FindImage("���60")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("���60"),
		Vector2(IMAGEMANAGER->FindImage("���60")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("���60")->GetSize().y / 2));

	fifthFloorStage::render();

	if (_isStopToRead)
		TEXTMANAGER->renderText();

	if (_isClick)
	{
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 250, camera.y));	//����
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x + 250, camera.y));	//������
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x, camera.y - 150));	//��
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x, camera.y + 150));	//�Ʒ�

		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 300, 345, L"��´�.", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 117, 345, L"�ƹ��͵� ���� �ʴ´�.", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 50, 195, L"�̴´�.", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 50, 495, L"���´�.", RGB(255, 255, 255), 0.85f, 27);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
	}
}

void garden_5f::Collision()
{
}

void garden_5f::load(LOCATION _location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/5f_garden.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute != PLAYER) continue;

		//�ʱ� ��ġ�� ����ش�.
		switch (_location)
		{
		case LOCATION_1:
			_player->setStart((DOORTODININGROOM + 1) % TILEX, (DOORTODININGROOM + 1) / TILEX);
			break;
		case LOCATION_2:
			_player->setStart((DOORTOGARDENTOBOSS + TILEX) % TILEX, (DOORTOGARDENTOBOSS + TILEX) / TILEX);
			break;		
		case LOCATION_3:
			_player->setStart((DOORTOPRISON - 1) % TILEX, (DOORTOPRISON - 1) / TILEX);
			break;
		case LOCATION_DEFAULT: default:
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}
		break;
	}
	CloseHandle(file);
}

void garden_5f::setTrigger()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// Ŭ���ൿ Ʈ����
	{
		if (IntersectRectToRect(&_tiles[FLOWER].rc, &_player->getSearchRc()))
		{
			switch (_co)
			{
			case FIRST:
				_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_garden_flower.txt");
				_isStopToRead = true;
				_co = SECOND;
				break;
			case SECOND:
				_isClick = true;
				break;
			}
		}
		if (IntersectRectToRect(&_tiles[NEKO].rc, &_player->getSearchRc()))
		{
			cout << "���̺�!" << endl;
		}
		if (IntersectRectToRect(&_tiles[TREE].rc, &_player->getSearchRc()) ||
			IntersectRectToRect(&_tiles[TREE + 1].rc, &_player->getSearchRc()))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_garden_tree.txt");
			_isStopToRead = true;
		}
		if (IntersectRectToRect(&_tiles[TEE].rc, &_player->getSearchRc()))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_garden_tee.txt");
			_isStopToRead = true;
		}
	}

	if (IntersectRectToRect(&_tiles[DOORTOGARDENTOBOSS].rc, &_player->getPlayerFrc()))
	{
		_isChangeScene = true;
		_vFrameTile[1].isTrigger = true;
		sceneChange("gardenToBoss_5f", CHRDIREC_UP, LOCATION_DEFAULT);
	}
	if (IntersectRectToRect(&_tiles[DOORTOPRISON].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTOPRISON + TILEX].rc, &_player->getPlayerFrc()))
	{
		_isChangeScene = true;
		sceneChange("prison_5f", CHRDIREC_RIGHT, LOCATION_DEFAULT);
	}
	if (IntersectRectToRect(&_tiles[DOORTODININGROOM].rc, &_player->getPlayerFrc()) ||
		IntersectRectToRect(&_tiles[DOORTODININGROOM + TILEX].rc, &_player->getPlayerFrc()))
	{
		_isChangeScene = true;
		sceneChange("diningRoom_5f", CHRDIREC_LEFT, LOCATION_DEFAULT);
	}
}

void garden_5f::setChoiceScene()
{
	if (_isClick)
	{
		rcAlphaChange();
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
			_rc = RectMakePivot(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
			_rc = RectMakePivot(Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_UP))
			_rc = RectMakePivot(Vector2(WINSIZEX / 2, WINSIZEY / 2 - 150), Vector2(270, 75), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
			_rc = RectMakePivot(Vector2(WINSIZEX / 2, WINSIZEY / 2 + 150), Vector2(270, 75), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			_isClick = false;
			if (IntersectRectToRect(&_rc, &_correct_rc))
				cout << "����!" << endl;
			else
				cout << "����!" << endl;
		}
	}
}

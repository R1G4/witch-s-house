#include "stdafx.h"
#include "hallway.h"
#include "Player.h"


hallway::hallway()
{
}


hallway::~hallway()
{
}

HRESULT hallway::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_player->setDirec(_chrdirection);

	//Ÿ�� �ҷ�����
	load(_location);

	//�׽�Ʈ�� �־�� ���� �����̸� �־����� �ʱ�ȭ �ؾ��� ���Ŀ� �����!
	//STAGEMEMORYMANAGER->setIsBearPut(true);
	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());

	firstFloorStage::init();

	_trigger = NONE;

	return S_OK;
}

void hallway::release()
{
	firstFloorStage::release();
}

void hallway::update()
{
	//������ �ε��� ����
	setFrameIndex();

	switch (_trigger)
	{
		case hallway::NONE:
			firstFloorStage::update();
			firstFloorStage::setAlpha();
			Collision();
			break;
		case hallway::DOOR_LEFT_OPEN:
			firstFloorStage::sceneChange("entrance", CHRDIREC_LEFT, LOCATION_1);
			break;
		case hallway::DOOR_RIGHT_TOP_OPEN:
			firstFloorStage::sceneChange("boxRoom", CHRDIREC_RIGHT, LOCATION_DEFAULT);
			break;
		case hallway::DOOR_RIGHT_BOTTOM_OPEN:
			firstFloorStage::sceneChange("bearRoom", CHRDIREC_RIGHT, LOCATION_DEFAULT);
			break;
		case hallway::CLOCK:
			firstFloorStage::setAlpha();
			_vScript = TEXTMANAGER->loadFile("dialog/1f/1f_hallwayClock.txt");

			if (KEYMANAGER->isOnceKeyUp(VK_SPACE) || KEYMANAGER->isOnceKeyUp('x'))
			{
				TEXTMANAGER->clearScript();
				_trigger = NONE;
				_vScript.clear();
			}
			break;
		default:
			_trigger = NONE;
			firstFloorStage::update();
			firstFloorStage::setAlpha();
			Collision();
			break;
	}	
	//ī�޶� ���� ������Ʈ
	firstFloorStage::cameraUpdate();
}

void hallway::render()
{
	//��� ����
	IMAGEMANAGER->FindImage("���11")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("���11"),
		Vector2(IMAGEMANAGER->FindImage("���11")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("���11")->GetSize().y / 2));

	firstFloorStage::render();
}

void hallway::Collision()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			int index = i * TILEX + j;
			//��� Ÿ�ϰ� �浹 ���� ���
			if (!IntersectRectToRect(&_tiles[index].rc, &_player->getPlayerFrc())) continue;

			//Ÿ�� �浹(�̵��� ���ϴ� Ÿ��)�� �����Ƿ� ������ Ŭ�������� ������.
			firstFloorStage::tileCollision(i, j);

			//�ش� Ÿ���� �ӿ� ����
			//���Ŀ� Ÿ�� �Ӽ� ���ܰ� ���� ��� ����ġ������ if������ ���� �� ������
			switch (_tiles[index].terrain)
			{
			case TR_TRIGGER:
				//�������
				//Ʈ���� �޾ƿ���
				_trigger = index == 1036 ? DOOR_LEFT_OPEN :
						   index == DOOR_LEFT_OPEN ? DOOR_LEFT_OPEN :
					       index == 621 ? DOOR_RIGHT_TOP_OPEN :
						   index == DOOR_RIGHT_TOP_OPEN ? DOOR_RIGHT_TOP_OPEN :
						   index == 1041 ? DOOR_RIGHT_BOTTOM_OPEN:
						   index == DOOR_RIGHT_BOTTOM_OPEN ? DOOR_RIGHT_BOTTOM_OPEN : NONE;

				if (_trigger == DOOR_RIGHT_TOP_OPEN || _trigger == DOOR_RIGHT_BOTTOM_OPEN)
					autoSound("openDoarLong");
				else if(_trigger == DOOR_LEFT_OPEN)
					autoSound("openDoarShort");

				if (((TRIGGER)index == CLOCK  || (TRIGGER)index == 499) && KEYMANAGER->isOnceKeyUp(VK_SPACE))
					_trigger = CLOCK;
			}
		}
	}
}

void hallway::load(LOCATION location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/1f_hallway.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute != PLAYER)
			continue;

		//�ʱ� ��ġ�� ����ش�.
		switch (location)
		{
		case LOCATION_1:
			_player->setStart(560 % TILEX, 560 / TILEX);
			break;
		case LOCATION_2:
			_player->setStart(980 % TILEX, 980 / TILEX);
			break;
		case LOCATION_DEFAULT: default:
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}
	}
	CloseHandle(file);
}
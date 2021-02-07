#include "stdafx.h"
#include "gardenToBoss_5f.h"
#include "Player.h"

HRESULT gardenToBoss_5f::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	_real_location1 = GARDEN_TO_BOSS;
	_player->setDirec(_chrdirection);

	//Ÿ�� �ҷ�����
	load(_location);

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	fifthFloorStage::init();

	// ���̾�α� ����

	return S_OK;
}

void gardenToBoss_5f::release()
{
}

void gardenToBoss_5f::update()
{
	if (!_isStopToRead)
	{
		fifthFloorStage::update();
		setFrameIndex();

		//ī�޶� ���� ������Ʈ
		cameraUpdate();
		setTrigger();
	}
	else
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// Ŭ���ൿ Ʈ����
		{
			_isStopToRead = TEXTMANAGER->setNextScript(true);
		}
	}
}

void gardenToBoss_5f::render()
{	
	//��� ����
	IMAGEMANAGER->FindImage("���72")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("���72"),
		Vector2(IMAGEMANAGER->FindImage("���72")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("���72")->GetSize().y / 2));

	fifthFloorStage::render();

	if (_isStopToRead)
		TEXTMANAGER->renderText();
}

void gardenToBoss_5f::Collision()
{
}

void gardenToBoss_5f::load(LOCATION _location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/5f_garden_to_boss.map", GENERIC_READ, NULL, NULL,
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
			_player->setStart(i % TILEX, i / TILEX);
			break;
		}
	}
	CloseHandle(file);
}

void gardenToBoss_5f::setTrigger()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// Ŭ���ൿ Ʈ����
	{
		_isStopToRead = TEXTMANAGER->setNextScript(true);

		if (IntersectRectToRect(&_tiles[BOOK].rc, &_player->getSearchRc()))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_gardenToBossBook.txt");
			_isStopToRead = true;
		}
		if (IntersectRectToRect(&_tiles[NEKO].rc, &_player->getSearchRc()))
		{
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_gardenToBossNeko.txt");
			_isStopToRead = true;
			cout << "���̺�!" << endl;
		}
	}

	if (IntersectRectToRect(&_tiles[DOORTOBOSS].rc, &_player->getPlayerFrc()))
	{
		_isChangeScene = true;
		_vFrameTile[0].isTrigger = true;
		sceneChange("BossStage1");
		fifthFloorStage::release();
	}
	if (IntersectRectToRect(&_tiles[DOORTOGARDEN].rc, &_player->getPlayerFrc()))
	{
		_isChangeScene = true;
		sceneChange("garden_5f", CHRDIREC_DOWN, LOCATION_2);
	}
}

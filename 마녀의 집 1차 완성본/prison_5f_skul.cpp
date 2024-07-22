#include "stdafx.h"
#include "prison_5f_skul.h"
#include "Player.h"

HRESULT prison_5f_skul::init(CHRDIRECTION _chrdirection, LOCATION _location)
{
	SOUNDMANAGER->play("����1");
	_real_location1 = PRISON_SKUL;
	_player->setDirec(_chrdirection);

	//Ÿ�� �ҷ�����
	load(_location);

	_tiles[SKUL_1].obj = OBJ_NONE;
	_tiles[SKUL_2].obj = OBJ_NONE;
	_tiles[SKUL_3].obj = OBJ_NONE;
	_tiles[SKUL_4].obj = OBJ_NONE;

	getMemory();

	camera = Vector2(_player->getPlayerLocX(), _player->getPlayerLocY());
	fifthFloorStage::init();
	_sound = false;
	return S_OK;
}

void prison_5f_skul::release()
{
	SOUNDMANAGER->stop("����1");
	_sound = false;
}

void prison_5f_skul::update()
{
	if(!_isStopToRead)
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

			if (_isSkul)
			{
				SOUNDMANAGER->play("getItem");
				_tiles[SKUL_5].obj = OBJ_NONE;
			}
		}
	}
}

void prison_5f_skul::render()
{	
	//��� ����
	IMAGEMANAGER->FindImage("���67")->SetAlpha(_sceneAlpha);
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("���67"),
		Vector2(IMAGEMANAGER->FindImage("���67")->GetSize().x / 2 + 480,
			IMAGEMANAGER->FindImage("���67")->GetSize().y / 2));

	fifthFloorStage::render();

	if (_isStopToRead)
		TEXTMANAGER->renderText();
}

void prison_5f_skul::Collision()
{
}

void prison_5f_skul::load(LOCATION _location)
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/5f_prison_1.map", GENERIC_READ, NULL, NULL,
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

void prison_5f_skul::setTrigger()
{
	FloatRect pSRc = _player->getSearchRc();
	FloatRect pFrc = _player->getPlayerFrc();

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	// Ŭ���ൿ Ʈ����
	{
		if (IntersectRectToRect(&_tiles[SKUL_1].rc, &pSRc) &&
			STAGEMEMORYMANAGER->getIsGetSkul1())
		{
			SOUNDMANAGER->play("�ذ�α�");
			STAGEMEMORYMANAGER->setIsSkul1(true);
			_tiles[SKUL_1].obj = OBJ_LOOK;
		}
		if (IntersectRectToRect(&_tiles[SKUL_2].rc, &pSRc) &&
			STAGEMEMORYMANAGER->getIsGetSkul2())
		{
			SOUNDMANAGER->play("�ذ�α�");
			STAGEMEMORYMANAGER->setIsSkul2(true);
			_tiles[SKUL_2].obj = OBJ_LOOK;
		}
		if (IntersectRectToRect(&_tiles[SKUL_3].rc, &pSRc) &&
			STAGEMEMORYMANAGER->getIsGetSkul3())
		{
			SOUNDMANAGER->play("�ذ�α�");
			STAGEMEMORYMANAGER->setIsSkul3(true);
			_tiles[SKUL_3].obj = OBJ_LOOK;
		}
		if (IntersectRectToRect(&_tiles[SKUL_4].rc, &pSRc) && 
			STAGEMEMORYMANAGER->getIsGetSkul4())
		{
			SOUNDMANAGER->play("�ذ�α�");
			STAGEMEMORYMANAGER->setIsSkul4(true);
			_tiles[SKUL_4].obj = OBJ_LOOK;
		}
		if (IntersectRectToRect(&_tiles[SKUL_5].rc, &pSRc))
		{
			STAGEMEMORYMANAGER->setIsSkul5(true);
			STAGEMEMORYMANAGER->setIsGetSkul4(true);
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_skul.txt");
			_isStopToRead = true;
			_isSkul = true;
		}
		if (IntersectRectToRect(&_tiles[LEVER].rc, &pSRc) && 
			_tiles[SKUL_1].obj == OBJ_LOOK &&
			_tiles[SKUL_2].obj == OBJ_LOOK &&
			_tiles[SKUL_3].obj == OBJ_LOOK &&
			_tiles[SKUL_4].obj == OBJ_LOOK)
		{
			SOUNDMANAGER->play("����ġ");
			STAGEMEMORYMANAGER->setIsLever(true);
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_lever_1.txt");
			_isStopToRead = true;
		}
		else if (IntersectRectToRect(&_tiles[LEVER].rc, &pSRc))
		{
			SOUNDMANAGER->play("����ġ");
			_vScript = TEXTMANAGER->loadFile("dialog/5f/5f_prison_lever.txt");
			_isStopToRead = true;
		}
	}

	if (IntersectRectToRect(&_tiles[DOORTOPRISON].rc, &pFrc) ||
		IntersectRectToRect(&_tiles[DOORTOPRISON + 1].rc, &pFrc) ||
		IntersectRectToRect(&_tiles[DOORTOPRISON - 1].rc, &pFrc))
	{
		if (!_sound)
			SOUNDMANAGER->play("ö��");
		_sound = true;
		_isChangeScene = true;
		sceneChange("prison_5f", CHRDIREC_LEFT, LOCATION_1);
	}
}

void prison_5f_skul::getMemory()
{
	if (STAGEMEMORYMANAGER->getIsSkul1()) _tiles[SKUL_1].obj = OBJ_LOOK;
	if (STAGEMEMORYMANAGER->getIsSkul2()) _tiles[SKUL_2].obj = OBJ_LOOK;
	if (STAGEMEMORYMANAGER->getIsSkul3()) _tiles[SKUL_3].obj = OBJ_LOOK;
	if (STAGEMEMORYMANAGER->getIsSkul4()) _tiles[SKUL_4].obj = OBJ_LOOK;
	if (STAGEMEMORYMANAGER->getIsSkul5()) _tiles[SKUL_5].obj = OBJ_NONE;
}

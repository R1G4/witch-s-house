#include "stdafx.h"
#include "thirdTo4thLoad.h"

thirdTo4thLoad::thirdTo4thLoad()
{
}

thirdTo4thLoad::~thirdTo4thLoad()
{
}

HRESULT thirdTo4thLoad::init()
{
	IMAGEMANAGER->AddFrameImage("SavePoint", L"Image/mapTool/saveCat.png", 16, 4);
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

	_player = new Player;
	load();

	_player->init();
	_player->setState(CHR_IDLE);
	_player->setDirec(CHRDIREC_DOWN);
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	CAMERAMANAGER->setCamera(camera);

	_count = 0;
	_frame = 0;
	_candleFrame = 0;
	_eyesFrameX = 0;
	_eyesFrameY = 0;
	_moveSword = false;

	return S_OK;
}

void thirdTo4thLoad::release()
{
}

void thirdTo4thLoad::update()
{

	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();

	CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));
	_player->update();
	tileCollision();
	changeScene();

}

void thirdTo4thLoad::render()
{
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

	//Z-order 벡터에 데이터를 넣어주는 과정
	ZORDER->insert(_player->getPlayerFrc().left, _player->getPlayerFrc().top, ZPLAYER);

	//오브젝트를 넣어주는 과정
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].obj != OBJ_NONE)
		{
			//if (_tiles[i].keyName == "obj5" || _tiles[i].keyName == "obj9")
			ZORDER->insert(_tiles[i].rc.left, _tiles[i].rc.top, IMAGEMANAGER->FindImage(_tiles[i].keyName), ZOBJECT);
		}
	}

	//정렬된 순서대로 랜딩
	//각 인덱스의 타입을 확인하여 타입에 따라 이미지를 랜딩하도록 설계함
	for (int i = 0; i < ZORDER->getZorder().size(); i++)
	{
		if (ZORDER->getZorder()[i].type == ZPLAYER)_player->render();

		if (ZORDER->getZorder()[i].type == ZOBJECT)
		{
			CAMERAMANAGER->render(ZORDER->getZorder()[i].img,
				Vector2(ZORDER->getZorder()[i].x + TILESIZE / 2, ZORDER->getZorder()[i].y + 48 - ZORDER->getZorder()[i].img->GetSize().y / 2));


			//19,32
			/*if (_player->getPlayerFrc().right / TILESIZE >= 19)
			{

				IMAGEMANAGER->FindImage(ZORDER->getZorder()[i].keyName)->SetAlpha(1);
				ZORDER->getZorder()[i].keyName = "061";
			}
			else
			{
				IMAGEMANAGER->FindImage(ZORDER->getZorder()[i].keyName)->SetAlpha(0);
			}*/
		}

		if (ZORDER->getZorder()[i].type == ZFRAMEOBJ)
		{
			CAMERAMANAGER->render(ZORDER->getZorder()[i].img,
				Vector2(ZORDER->getZorder()[i].x + TILESIZE / 2, ZORDER->getZorder()[i].y + 48 - ZORDER->getZorder()[i].img->GetSize().y / 2));
		}


	}

	//zorder 벡터를 초기화해줌 안하면 느려짐
	ZORDER->release();

}

void thirdTo4thLoad::changeScene()
{
	//4층홀로 이동
	if (_player->getPlayerFrc().top / TILESIZE <= 6.2f
		&& (_player->getPlayerFrc().right / TILESIZE >= 36 && _player->getPlayerFrc().right / TILESIZE <= 37))
	{
		SCENEMANAGER->changeScene("4층홀");
	}

}

void thirdTo4thLoad::tileCollision()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			FloatRect pFrc = _player->getPlayerFrc();
			if (IntersectRectToRect(&pFrc, &_tiles[i*TILEX + j].rc) && _tiles[i*TILEX + j].isCollider)
			{
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
		}
	}
}

void thirdTo4thLoad::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/3f_to_4f.map", GENERIC_READ, NULL, NULL,
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
	}

	CloseHandle(file);
}
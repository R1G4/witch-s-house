#include "stdafx.h"
#include "thirdOnewayLoad.h"

thirdOnewayLoad::thirdOnewayLoad()
{
}

thirdOnewayLoad::~thirdOnewayLoad()
{
}

HRESULT thirdOnewayLoad::init()
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

void thirdOnewayLoad::release()
{
}

void thirdOnewayLoad::update()
{
	_count++;
	if (_count % 8 == 0)
	{
		_frame++;
		_candleFrame++;
		_eyesFrameX++;
		_eyesFrameY++;
		if (_frame > 15) _frame = 0;
		if (_candleFrame >= 3) _candleFrame = 0;

		if (_eyesFrameX > 2)
		{
			_eyesFrameX = 0;
		}
		if (_eyesFrameY > 3)
		{
			_eyesFrameY = 0;
		}
	}

	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();

	CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));
	_player->update();
	tileCollision();
	trigger();
	changeScene();

	if (_count % 10 == 0) cout << _player->getPlayerFrc().bottom / TILESIZE << endl;


}

void thirdOnewayLoad::render()
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
	_player->render();
	//CAMERAMANAGER->FrameRender(IMAGEMANAGER->FindImage("SavePoint"), Vector2(920, 1680), _frame, 2);

	//오브젝트 랜더
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;
			//IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->SetAlpha(0.5);
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top));
		}
	}
	//프레임이미지 랜더
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].attribute == NONE)continue;
			if (_tiles[i*TILEX + j].attribute == PLAYER)continue;


			if (_tiles[i*TILEX + j].attribute == OBJ)
			{
				CAMERAMANAGER->FrameRender(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
					Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top), _candleFrame, 0);
			}

			if (_tiles[i*TILEX + j].attribute == ENEMY)
			{
				CAMERAMANAGER->FrameRender(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
					Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top), _eyesFrameX, _eyesFrameY);

				//눈알 에너미 이미지가 트리거 발동전에는 안보이다가 발동하면 이미지 보이게
				if (_player->getPlayerFrc().bottom / TILESIZE >= 22)
				{

					IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->SetAlpha(1);
				}
				else
				{
					IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->SetAlpha(0);

				}
			}

		}
	}

}

void thirdOnewayLoad::changeScene()
{
	//플레이어가 옆타일로 이동시 데드씬으로
	if (_player->getPlayerFrc().left / TILESIZE <= 21 || _player->getPlayerFrc().right / TILESIZE >= 23)
	{
		SCENEMANAGER->changeScene("thirdOnewayDead");
	}


}

void thirdOnewayLoad::trigger()
{
	for (int i = 0; i < (TILEX * TILEY); ++i)
	{
		//플레이어의 타일 인덱스의 바텀이 10보다 커지면 sword 트리거 on
		if (_player->getPlayerFrc().bottom / TILESIZE >= 10)
		{
			_moveSword = true;
		}
		//플레이어의 타일 인덱스의 바텀이 22보다 커지면 eyes 트리거 on
		if (_player->getPlayerFrc().bottom / TILESIZE >= 22)
		{
			_moveEyes = true;
		}


		if (_moveSword)
		{
			if (_tiles[i].terrain == TR_TRIGGER && _tiles[i].obj == OBJ_LOOK)
			{
				_tiles[i].rc.bottom -= 10;
				_tiles[i].rc.top -= 10;
			}
		}

		if (_moveEyes)
		{
			if (_tiles[i].terrain == TR_TRIGGER && _tiles[i].attribute == ENEMY)
			{
				_tiles[i].rc.bottom += 13;
				_tiles[i].rc.top += 13;
			}
		}


	}



}


void thirdOnewayLoad::tileCollision()
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


				SCENEMANAGER->changeScene("thirdFrogOutRoom");

			}
		}
	}
}

void thirdOnewayLoad::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/3f_sword.map", GENERIC_READ, NULL, NULL,
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
		/*if (_tiles[i].attribute == ENEMY)
		{
			bossLocX = i % TILEX;
			bossLocY = i / TILEX;
		}*/
		if (_tiles[i].attribute == OBJ)
		{

		}
	}

	CloseHandle(file);
}
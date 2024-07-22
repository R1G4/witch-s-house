#include "stdafx.h"
#include "soundStageW.h"

soundStageW::soundStageW()
{
}

soundStageW::~soundStageW()
{
}

HRESULT soundStageW::init()
{
	_rcAlpha = 1.0f;
	IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/타일.png", 7, 2);
	IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
	IMAGEMANAGER->AddFrameImage("SavePoint", L"Image/mapTool/saveCat.png", 16, 4);
	IMAGEMANAGER->AddImage("닫힌 오르골", L"Image/orgole-C.png");
	IMAGEMANAGER->AddImage("열린 오르골", L"Image/orgole-O.png");


	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

	_player = new Player;
	load();
	_player->init();
	_player->setDirec(CHRDIREC_LEFT);
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	CAMERAMANAGER->setCamera(camera);

	_isStopToRead = false;
	_isClick = false;
	_disCover = false;



	mapChange.x = WINSIZEX / 2 + 343 + 48 * 12;
	mapChange.y = WINSIZEY / 2 - 120 + 48 * 10;

	mapChange.rc = RectMakeCenter(mapChange.x, mapChange.y, 24, 72);


	return S_OK;
}

//HRESULT soundStageW::init(CHRDIRECTION _CHRDIRECTION)
//{
//	return S_OK;
//}

void soundStageW::release()
{
}

void soundStageW::update()
{
	if (!_isClick && !_isStopToRead)
	{
		camera.x = _player->getPlayerLocX();
		camera.y = _player->getPlayerLocY();

		CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));
		_player->update();
		tileCollision();
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			_isStopToRead = TEXTMANAGER->setNextScript(true);
			FloatRect pSRc = _player->getSearchRc();
			if (IntersectRectToRect(&_tiles[TRIGGER].rc, &pSRc))
			{
				_isClick = true;
			}
		}
	}
	else if (_isStopToRead)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			_isStopToRead = TEXTMANAGER->setNextScript(true);
		}
	}
	_correct_rc = RectMakePivot(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);
	if (_rcAlpha >= 0.9f)
		_rcAlphaChange = -0.03f;

	if (_rcAlpha <= 0.35f)
		_rcAlphaChange = 0.03f;
	_rcAlpha += _rcAlphaChange;

	if (_isClick)
	{
		if (!_open && !STAGEMEMORYMANAGER->getIsGetTae())
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
			{
				_left = true;
				_right = false;
				_choose = true;
				_rc = RectMakePivot(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);

			}
			if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
			{
				_left = false;
				_right = true;
				_choose = true;
				_rc = RectMakePivot(Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);

			}
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				if (_left)
				{
					STAGEMEMORYMANAGER->setIsOpenFake(true);
					STAGEMEMORYMANAGER->setIsOpen(false);
					_vScript = TEXTMANAGER->loadFile("dialog/4f/4f_Open.txt");
					_isStopToRead = true;
					_open = true;
				}
				if (_right)
				{
					STAGEMEMORYMANAGER->setIsOpenFake(false);
					STAGEMEMORYMANAGER->setIsOpen(false);
				}
				_choose = false;
				_isClick = false;

			}
		}


	}
	if (!_open && STAGEMEMORYMANAGER->getIsGetTae())
	{


		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			_left = true;
			_right = false;
			_up = false;
			_choose = true;
			_rc = RectMakePivot(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);

		}
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			_left = false;
			_right = true;
			_up = false;
			_choose = true;
			_rc = RectMakePivot(Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);

		}
		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			_left = false;
			_right = false;
			_up = true;
			_choose = true;
			_rc = RectMakePivot(Vector2(WINSIZEX / 2, WINSIZEY / 2 - 125), Vector2(270, 75), Pivot::Center);

		}
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			if (_left)
			{
				STAGEMEMORYMANAGER->setIsOpenFake(true);
				STAGEMEMORYMANAGER->setIsOpen(false);
				_isStopToRead = true;
				_vScript = TEXTMANAGER->loadFile("dialog/4f/4f_Open.txt");
				_open = true;
			}
			if (_right)
			{
				STAGEMEMORYMANAGER->setIsOpenFake(false);
				STAGEMEMORYMANAGER->setIsOpen(false);

			}
			if (_up)
			{
				if (STAGEMEMORYMANAGER->getIsPiano())
				{
					_isStopToRead = true;
					_vScript = TEXTMANAGER->loadFile("dialog/4f/4f_Open.txt");
				}
				STAGEMEMORYMANAGER->setIsOpenFake(false);
				STAGEMEMORYMANAGER->setIsOpen(true);
				STAGEMEMORYMANAGER->setIsGetTae(false);
				STAGEMEMORYMANAGER->setIsGetAkbo(true);
				ITEMMANAGER->useItem("obj33");
				
				if (!STAGEMEMORYMANAGER->getIsPiano())
				{
					_isStopToRead = true;
					_vScript = TEXTMANAGER->loadFile("dialog/4f/4f_Open_tae.txt");
					ITEMMANAGER->addItem("obj44");
				}
				
				_open = true;
			}
			_choose = false;
			_isClick = false;


		}

	}
	else if (_open)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			_left = true;
			_right = false;
			_choose = true;
			_disCover = true;
			_rc = RectMakePivot(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);

		}
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			_left = false;
			_right = true;
			_choose = true;
			_disCover = false;
			_rc = RectMakePivot(Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2), Vector2(270, 75), Pivot::Center);



		}
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			if (_left)
			{
				if (STAGEMEMORYMANAGER->getIsOpenFake())
				{
					_isStopToRead = true;
					_vScript = TEXTMANAGER->loadFile("dialog/4f/4f_Close.txt");
					STAGEMEMORYMANAGER->setIsOpenFake(false);
					_open = false;
				}

				if (STAGEMEMORYMANAGER->getIsOpen())
				{
					if (!STAGEMEMORYMANAGER->getIsPiano())
					{
						_isStopToRead = true;
						_vScript = TEXTMANAGER->loadFile("dialog/4f/4f_Close_tae_akbo.txt");
						ITEMMANAGER->addItem("obj33");
						ITEMMANAGER->useItem("obj44");
						STAGEMEMORYMANAGER->setIsGetTae(true);
						STAGEMEMORYMANAGER->setIsOpen(false);
						STAGEMEMORYMANAGER->setIsGetAkbo(false);
						_open = false;
					}

					if (STAGEMEMORYMANAGER->getIsPiano())
					{
						_isStopToRead = true;
						_vScript = TEXTMANAGER->loadFile("dialog/4f/4f_Close_tae.txt");
						ITEMMANAGER->addItem("obj33");
						STAGEMEMORYMANAGER->setIsGetTae(true);
						STAGEMEMORYMANAGER->setIsOpen(false);
						_open = false;
					}
				
				}
			}
			if (_right)
			{
				_isClick = false;
			}


			_choose = false;
			_isClick = false;
	
		}

	}

}



void soundStageW::render()
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
				CAMERAMANAGER->renderRc(mapChange.rc, D2D1::ColorF::White, 1.0f, 24);

			}

		}
	}
	if (!_open)
	{
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("닫힌 오르골"), Vector2(WINSIZEX / 2 + 12 + 48 * 10, WINSIZEY / 2 + 36));
	}
	else
	{
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("열린 오르골"), Vector2(WINSIZEX / 2 + 12 + 48 * 10, WINSIZEY / 2 + 36));
	}




	_player->render();
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2));
		}
	}
	if (_isStopToRead)
	{
		TEXTMANAGER->renderText();
	}
	if (_isClick)
	{
		if (!_open && !STAGEMEMORYMANAGER->getIsGetTae())
		{
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 250, camera.y));
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x + 250, camera.y));


			D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 300, 345, L"열어본다", RGB(255, 255, 255), 0.85f, 27);
			D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 117, 345, L"아무것도 하지 않는다", RGB(255, 255, 255), 0.85f, 27);
			if (_choose)
			{
				D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5f);
				D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
			}

		}
		if (!_open && STAGEMEMORYMANAGER->getIsGetTae())
		{
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 250, camera.y));
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x + 250, camera.y));
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x, camera.y - 125));


			D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 300, 345, L"열어본다", RGB(255, 255, 255), 0.85f, 27);
			D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 128, 220, L"태엽을 넣어 열어본다", RGB(255, 255, 255), 0.85f, 27);
			D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 117, 345, L"아무것도 하지 않는다", RGB(255, 255, 255), 0.85f, 27);
			if (_choose)
			{
				D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5f);
				D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
			}

		}
		if (_open)
		{
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 250, camera.y));
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x + 250, camera.y));

			D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 300, 345, L"닫는다", RGB(255, 255, 255), 0.85f, 27);
			D2DINS->GetInstance()->RenderText(WINSIZEX / 2 + 117, 345, L"아무것도 하지 않는다", RGB(255, 255, 255), 0.85f, 27);

			if (_choose)
			{
				D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5f);
				D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
			}

		}

	}
}

void soundStageW::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/4F_WROOM.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	_backGround = IMAGEMANAGER->FindImage(_tiles->backGroundName);

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

void soundStageW::tileCollision()
{
	FloatRect pFrc = _player->getPlayerFrc();

	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
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
	if (IntersectRectToRect(&pFrc, &mapChange.rc))
	{
		if (STAGEMEMORYMANAGER->getIsOpen())
		{
			SCENEMANAGER->changeScene("4층홀", CHRDIREC_RIGHT);
		}
		if (STAGEMEMORYMANAGER->getIsOpenFake())
		{
			SCENEMANAGER->changeScene("4층함정", CHRDIREC_RIGHT);
		}
		if (!STAGEMEMORYMANAGER->getIsOpenFake())
		{
			SCENEMANAGER->changeScene("4층홀", CHRDIREC_RIGHT);
		}
	}
}

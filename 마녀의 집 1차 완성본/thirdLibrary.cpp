#include "stdafx.h"
#include "thirdLibrary.h"

thirdLibrary::thirdLibrary()
{
}

thirdLibrary::~thirdLibrary()
{
}

HRESULT thirdLibrary::init()
{
	IMAGEMANAGER->AddImage("3f_doar", L"Image/obj/3f_2_doar.png");
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

	_player = new Player;
	load();

	_player->init();
	_player->setState(CHR_IDLE);
	_player->setDirec(CHRDIREC_RIGHT);
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	CAMERAMANAGER->setCamera(camera);

	_count = 0;
	_frame = 0;
	_candleFrame = 0;
	_dialogue = false;
	_isStopToRead = false;

	return S_OK;
}

void thirdLibrary::release()
{
}

void thirdLibrary::update()
{
	_count++;
	if (_count % 4 == 0)
	{
		_frame++;
		_candleFrame++;
		if (_frame > 15)
		{
			_frame = 0;
		}
		if (_candleFrame >= 3) _candleFrame = 0;
	}

	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();

	CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));
	_player->update();
	tileCollision();
	readBook();

	changeScene();

	if (_count % 10 == 0) cout << _player->getPlayerFrc().left / TILESIZE << endl;
}

void thirdLibrary::render()
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
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("3f_doar"), Vector2(WINSIZEX / 2 + 130, WINSIZEY / 2 - 60));
	_player->render();

	//타일 오브젝트 or 프레임이미지 랜더 
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
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].attribute == NONE)continue;
			if (_tiles[i*TILEX + j].attribute == PLAYER)continue;
			//IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->SetAlpha(0.5);
			CAMERAMANAGER->FrameRender(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top), _candleFrame, 0);
		}
	}


	//다이어로그 켜졌을때
	if (_dialogue)
	{
		if (_isStopToRead)
			TEXTMANAGER->renderText();
	}
}

void thirdLibrary::changeScene()
{
	//다음 스테이지로 이동
	if (_player->getPlayerFrc().bottom / TILESIZE >= 10.7f)
	{
		SOUNDMANAGER->stop("다운받은거2");
		SOUNDMANAGER->play("openDoarLong");
		SCENEMANAGER->changeScene("thirdOnewayLoad");
	}

	//이전  스테이지로 이동
	if (_player->getPlayerFrc().left / TILESIZE <= 16.3f)
	{
		SOUNDMANAGER->play("openDoarLong");
		SCENEMANAGER->changeScene("thirdMain", CHRDIREC_LEFT);

	}
}

void thirdLibrary::readBook()
{
	//책 읽기
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			//일지 읽을때
			if (IntersectRectToRect(&_player->getSearchRc(), &_tiles[i*TILEX + j].rc)
				&& _tiles[i*TILEX + j].terrain == TR_TRIGGER && _player->getPlayerFrc().right / TILESIZE <= 20)
			{
				if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
				{
					SOUNDMANAGER->play("nextPage");
					_dialogue = true;
					_isStopToRead = TEXTMANAGER->setNextScript(true);
					_vScript = TEXTMANAGER->loadFile("dialog/3f/3f_library_message.txt");
					_isStopToRead = true;
				}
			}
			//마녀의 책 읽을때
			if (IntersectRectToRect(&_player->getSearchRc(), &_tiles[i*TILEX + j].rc)
				&& _tiles[i*TILEX + j].terrain == TR_TRIGGER && _player->getPlayerFrc().right / TILESIZE >= 21)
			{
				if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
				{
					SOUNDMANAGER->play("openBook");
					_dialogue = true;
					_isStopToRead = TEXTMANAGER->setNextScript(true);
					_vScript = TEXTMANAGER->loadFile("dialog/3f/3f_library_book.txt");
					_isStopToRead = true;
				}
			}

			//다이어로그 켜져있을때 스페이스바 누르면 원래대로 돌아가게. 모든 다이어로그 다 포함
			if (_dialogue)
			{
				if (KEYMANAGER->isOnceKeyDown('B'))
				{
					_dialogue = false;
				}
			}
		}
	}
}

void thirdLibrary::tileCollision()
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

			}
		}
	}
}

void thirdLibrary::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/3f_library.map", GENERIC_READ, NULL, NULL,
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
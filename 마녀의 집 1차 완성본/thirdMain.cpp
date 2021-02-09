#include "stdafx.h"
#include "thirdMain.h"

thirdMain::thirdMain()
{
}

thirdMain::~thirdMain()
{
}

HRESULT thirdMain::init(CHRDIRECTION _chrdirection)
{
	_saveCat = IMAGEMANAGER->AddFrameImage("SavePoint", L"Image/mapTool/saveCat.png", 16, 4);
	IMAGEMANAGER->AddFrameImage("켜진초", L"Image/tempFrameImg/켜진초.png", 3, 0);
	IMAGEMANAGER->AddImage("loadTop", L"image/UI/loadTop.png");
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);
	_count = 0;
	_catFrameX = 0;
	_catFrameY = 0;
	_candleFrame = 0;

	//다른스테이지에서 씬전환 했을때 플레이어 모션이 left였다면
	if (_chrdirection == CHRDIREC_LEFT)
	{
		_player = new Player;
		_player->setStart(0, 0);

		load();

		_player->init();
		_player->setState(CHR_IDLE);
		_player->setDirec(CHRDIREC_LEFT);
		_player->setPLocaX(WINSIZEX / 2 + 520);		//setStart 0,0으로 두고 위치 잡아주기
		_player->setPLocaY(WINSIZEY / 2 + 280);

		camera.x = _player->getPlayerLocX();
		camera.y = _player->getPlayerLocY();
		CAMERAMANAGER->setCamera(camera);
		_dialogue = false;
	}
	//다른스테이지에서 씬전환 했을때 플레이어 모션이 right였다면
	if (_chrdirection == CHRDIREC_RIGHT)
	{
		_player = new Player;
		_player->setStart(0, 0);

		load();

		_player->init();
		_player->setState(CHR_IDLE);
		_player->setDirec(CHRDIREC_RIGHT);
		_player->setPLocaX(WINSIZEX / 2 + 230);
		_player->setPLocaY(WINSIZEY / 2 + 130);

		camera.x = _player->getPlayerLocX();
		camera.y = _player->getPlayerLocY();
		CAMERAMANAGER->setCamera(camera);
		_dialogue = false;
	}
	//다른스테이지에서 씬전환 했을때 플레이어 모션이 down였다면
	if (_chrdirection == CHRDIREC_DOWN)
	{
		_player = new Player;
		load();

		_player->init();
		_player->setState(CHR_IDLE);
		_player->setDirec(CHRDIREC_DOWN);

		camera.x = _player->getPlayerLocX();
		camera.y = _player->getPlayerLocY();
		CAMERAMANAGER->setCamera(camera);

		_dialogue = false;
	}

	return S_OK;
}

void thirdMain::release()
{
}

void thirdMain::update()
{
	_count++;
	if (_count % 4 == 0)
	{
		_catFrameX++;
		_candleFrame++;
		if (_catFrameX > 15) _catFrameX = 0;
		if (_candleFrame >= 3) _candleFrame = 0;

	}

	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();

	CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));
	_player->update();
	tileCollision();
	changeScene();
	save();
	//if(_count %10 == 0) cout << _player->getPlayerFrc().right / TILESIZE << endl;
}

void thirdMain::render()
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
	CAMERAMANAGER->FrameRender(IMAGEMANAGER->FindImage("SavePoint"), Vector2(WINSIZEX / 2 + 440, WINSIZEY / 2 + 210), _catFrameX, _catFrameY);
	CAMERAMANAGER->FrameRender(IMAGEMANAGER->FindImage("켜진초"), Vector2(WINSIZEX / 2 + 440, WINSIZEY / 2 + 90), _candleFrame, 0);

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

	if (_dialogue)
	{
		if (_isStopToRead) TEXTMANAGER->renderText();

		if (KEYMANAGER->isOnceKeyDown('B'))
		{
			_dialogue = false;
		}

	}
}

void thirdMain::changeScene()
{
	//개구리방으로 이동
	if (_player->getPlayerFrc().right / TILESIZE <= 17)
	{
		SOUNDMANAGER->play("openDoarLong", 0.8f);
		SCENEMANAGER->changeScene("thirdFrogRoom");

	}

	//서재로 이동
	if (_player->getPlayerFrc().right / TILESIZE >= 25.5f)
	{

		//if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		//{
		//	

		//	//아이템창에 개구리가 없다면
		//	if (ITEMMANAGER->getItemKinds("frog"))
		//	{
		//		SOUNDMANAGER->play("rockDoar");
		//		_dialogue = true;
		//		_isStopToRead = TEXTMANAGER->setNextScript(true);
		//		_vScript = TEXTMANAGER->loadFile("dialog/3f/3f_frog.txt");
		//		_isStopToRead = true;
		//	}
		//	else
		//	{
		//		SOUNDMANAGER->play("openDoarLong", 0.8f);
		//		SCENEMANAGER->changeScene("thirdLibrary");
		//	}
		//}

		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			//개구리가 있다면 씬전환
			if (ITEMMANAGER->KeyCheck("frog"))
			{
				SOUNDMANAGER->play("openDoarLong", 0.8f);
				SCENEMANAGER->changeScene("thirdLibrary");
			}
			//개구리가 없다면
			else
			{
				_dialogue = true;
				_isStopToRead = TEXTMANAGER->setNextScript(true);
				_vScript = TEXTMANAGER->loadFile("dialog/3f/3f_frog.txt");
				_isStopToRead = true;

			}
		}




		/*else
		{
			cout << "rrr" << endl;
			SOUNDMANAGER->play("openDoarLong", 0.8f);
			SCENEMANAGER->changeScene("thirdLibrary");
		}*/

	}

	/*if (_player->getPlayerFrc().right / TILESIZE >= 25.5f)
	{
		SOUNDMANAGER->play("openDoarLong", 0.8f);
		SCENEMANAGER->changeScene("thirdLibrary");
	}*/
}

//캐릭터, 콜라이더 타일 충돌했을때
void thirdMain::tileCollision()
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

///////////////////////////////////////////////////////////////////////////////////////
//고양이 눌렀을때 세이브창 뜨고 세이브 되게
void thirdMain::save()
{
	for (int i = 0; i < (TILEX*TILEY); ++i)
	{
		if (IntersectRectToRect(&_player->getSearchRc(), &_tiles[i].rc)
			&& _tiles[i].terrain == TR_TRIGGER)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				SOUNDMANAGER->play("cat");
				_catFrameY = 1;
				//요기
			}
		}
	}

}

///////////////////////////////////////////////////////////////////////////////////
void thirdMain::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/3f_main.map", GENERIC_READ, NULL, NULL,
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
#include "stdafx.h"
#include "thirdFrogRoom.h"

thirdFrogRoom::thirdFrogRoom()
{
}

thirdFrogRoom::~thirdFrogRoom()
{
}

HRESULT thirdFrogRoom::init()
{
	_bar = IMAGEMANAGER->AddImage("bar", L"Image/obj/bar2.png");
	_frog = IMAGEMANAGER->AddFrameImage("frog", L"Image/tempFrameImg/frog.png", 8, 4);
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

	_player = new Player;
	load();

	_player->init();
	_player->setState(CHR_IDLE);
	_player->setDirec(CHRDIREC_DOWN);

	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	CAMERAMANAGER->setCamera(camera);

	_rcAlpha = 1.0f;
	_rcAlphaChange = 0.03f;
	_x = WINSIZEX / 2;
	_y = WINSIZEY / 2;
	_x2 = WINSIZEX / 2 + 250;
	_y2 = WINSIZEY / 2 + 50;
	_rc = RectMakePivot(Vector2(_x - 250, _y - 8), Vector2(270, 75), Pivot::Center);

	_alpha = 255;
	_count = 0;
	_frameX = 0;
	_frameY = 0;
	_isText = false;

	_dialogue = false;
	_isStopToRead = false;

	return S_OK;
}

void thirdFrogRoom::release()
{
}

void thirdFrogRoom::update()
{
	_count++;
	if (_count % 12 == 0)
	{
		_frameX++;
		if (_frameX > 7)
		{
			_frameX = 0;
		}
	}
	//_count++;
	//if(_count %10 == 0) cout << _player->getPlayerFrc().bottom / TILESIZE << endl;

	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();

	CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));
	if (!_isText)
	{
		_player->update();
	}
	tileCollision();
	rcAlphaChange();
	changeScene();
	openText();
	readBook();
}

void thirdFrogRoom::render()
{
	//텍스트창이 오픈되어있지 않은 평범한 상태
	if (!_isText)
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

		for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEX; j++)
			{
				if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;
				CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
					Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2));
			}
		}
	}

	//텍스트창 오픈
	if (_isText)
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

		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 250, camera.y - 10));	//왼쪽
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x + 180, camera.y - 10));	//오른쪽
		D2DINS->GetInstance()->RenderText(330, 330, L"끄덕인다", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(740, 330, L"고개를 젓는다", RGB(255, 255, 255), 0.85f, 27);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
	}

	//다이어로그 켜졌을때
	if (_dialogue)
	{
		cout << "yyy" << endl;
		if (_isStopToRead)
			TEXTMANAGER->renderText();
	}


	CAMERAMANAGER->FrameRender(IMAGEMANAGER->FindImage("frog"), Vector2(_x2, _y2), _frameX, _frameY);

	_player->render();

}

void thirdFrogRoom::rcAlphaChange()
{
	if (_rcAlpha >= 0.9f)
		_rcAlphaChange = -0.03f;

	if (_rcAlpha <= 0.35f)
		_rcAlphaChange = 0.03f;

	_rcAlpha += _rcAlphaChange;
}

void thirdFrogRoom::changeScene()
{
	if (_player->getPlayerFrc().right / TILESIZE >= 23.5f)
	{
		SCENEMANAGER->changeScene("thirdMain");
	}
}
void thirdFrogRoom::openText()
{
	if (_player->getPlayerFrc().left / TILESIZE <= 21 && _player->getPlayerFrc().bottom / TILESIZE >= 7)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			//개구리 프레임x,y축 변경해줌
			_frameY = 2;
			_frameX = 2;
			_isText = true;
		}
	}

	if (_isText)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			_rc = RectMakePivot(Vector2(_x + 180, _y - 10), Vector2(270, 75), Pivot::Center);
			_text = RIGHT;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			_rc = RectMakePivot(Vector2(_x - 250, _y - 8), Vector2(270, 75), Pivot::Center);

			_text = LEFT;
		}
	}

	if (_text == LEFT)
	{
		if (KEYMANAGER->isOnceKeyDown('A'))
		{
			_isText = false;

		}
	}
	//좌측 텍스트 선택한다면
	if (_text == LEFT && !_isText)
	{
		if (_player->getPlayerFrc().left / TILESIZE <= _x2)
		{
			cout << _alpha << endl;
			_frameX = 2;
			_y2 - 20;
			_x2 += 1.3f;
			_alpha -= 5;
			_frog->SetAlpha(_alpha);
		}
	}
	//우측 텍스트 선택한다면
	if (_text == RIGHT)
	{
		if (_frameY == 2 && KEYMANAGER->isOnceKeyDown('A'))
		{
			_isText = false;

		}
		if (!_isText)
		{
			//개구리 돌리기 걍 넣어봤음 빼도 됌
			_alpha++;
			_frameX = 2;
			_frog->SetAngle(_alpha);

		}
	}
}

void thirdFrogRoom::readBook()
{
	//책 읽기
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (IntersectRectToRect(&_player->getSearchRc(), &_tiles[i*TILEX + j].rc)
				&& _tiles[i*TILEX + j].terrain == TR_TRIGGER && _player->getPlayerFrc().bottom / TILESIZE <= 7)
			{
				if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
				{
					_dialogue = true;
					_isStopToRead = TEXTMANAGER->setNextScript(true);
					_vScript = TEXTMANAGER->loadFile("dialog/3f/3f_book.txt");
					_isStopToRead = true;
				}
			}
			//다이어로그 켜져있을때 스페이스바 누르면 원래대로 돌아가게. 모든 다이어로그 다 포함
			if (_dialogue)
			{
				if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
				{
					_dialogue = false;
				}
			}
		}
	}
}

void thirdFrogRoom::tileCollision()
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

void thirdFrogRoom::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/3f_frogRoom2.map", GENERIC_READ, NULL, NULL,
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
		if (_tiles[i].attribute == OBJ)
		{

		}
	}
	CloseHandle(file);
}